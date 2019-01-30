import monitor.*;
import java.util.Random;

// ****************************************************************************

class Barberia extends AbstractMonitor{
	private Condition barbero = makeCondition();
	private Condition sala = makeCondition();
	private Condition silla = makeCondition();

	public void cortarPelo(){
		enter();
			if(!silla.isEmpty()){
				System.out.println("\033[32m " + "Silla ocupada. Cliente en espera.");
				sala.await();
			}
			System.out.println("\033[32m " + "Cliente avisa al barbero.");
			barbero.signal();
			silla.await();
		leave();
	}
	
	public void siguienteCliente(){
		enter();
			if (sala.isEmpty() && silla.isEmpty()){
				System.out.println("\033[34m " + "No hay clientes. Barbero en espera.");
				barbero.await();
			}
			System.out.println("\033[34m " + "Barbero coge cliente. Empieza a pelar.");
			sala.signal();
		leave();
	}

	public void finCliente(){
		enter();
			System.out.println("\033[33m " + "Barbero termina de pelar.");
			silla.signal();
		leave();
	}
}

// ****************************************************************************

class Cliente implements Runnable{
	public Thread thr;
	private Barberia barberia;
	
	public Cliente(Barberia barberia){
		thr = new Thread(this, "Cliente ");
		this.barberia = barberia;
	}

	public void run(){
		while (true){
			try{
				barberia.cortarPelo();
				aux.dormir_max(2000);
			}catch(Exception e){
				System.err.println("\033[31m Error en Cliente: " + e);
			}
		}
	}
}

// ****************************************************************************

class Barbero implements Runnable{
	public Thread thr;
	private Barberia barberia;
	
	public Barbero(Barberia barberia){
		this.barberia = barberia;
		thr = new Thread(this, "Barbero");
	}

	public void run(){
		while(true){
			try{
				barberia.siguienteCliente();
				aux.dormir_max(2500);
				barberia.finCliente();
			}catch(Exception e){
				System.err.println("\033[31m Error en Barbero: " + e);
			}
		}
	}
}

// ****************************************************************************

class aux{
	static Random aleatorio = new Random();
	
	static void dormir_max(int milisegundos){
		try{
			Thread.sleep(aleatorio.nextInt(milisegundos));
		}catch(InterruptedException e){
			System.err.println("\033[31m Error al dormir la hebra");
		}
	}
}

// ****************************************************************************

class MainBarberia{
	public static void main(String[] args){
		Barberia barberia = new Barberia();
		Barbero barbero = new Barbero(barberia);
		Cliente[] clientes = new Cliente[10];
		
		for (int i = 0; i < 10; i++)
			clientes[i] = new Cliente(barberia);

		barbero.thr.start();
		for (int i = 0; i < 10; i++)
			clientes[i].thr.start();
	}
}
