import monitor.*;
import java.util.Random;

// ****************************************************************************

class Estanco extends AbstractMonitor{
	private Condition sem_estanquero = makeCondition();
	private Condition[] sem_fumador = new Condition[3];
	private int existIngrediente = -1;

	public Estanco(){
		for (int i=0; i<3; i++)
			sem_fumador[i] = makeCondition();
	}

	public void obtenerIngrediente(int miIngrediente){
		enter();
			if(miIngrediente != existIngrediente)
				sem_fumador[miIngrediente].await();
			System.out.println("\033[34m " + "Fumando el fumador del ingrediente " + miIngrediente);
			existIngrediente = -1;        
			sem_estanquero.signal();
		leave();
	}
	
	public void ponerIngrediente(int ingrediente){
		enter();
			existIngrediente = ingrediente;
			System.out.println("\033[32m " + "El estanquero saca el ingrediente " + ingrediente);
			sem_fumador[ingrediente].signal();
		leave();
	}
	
	public void esperarRecogidaIngrediente(){
		enter();
		if(existIngrediente != -1)
			sem_estanquero.await();
		leave();
	}
}

// ****************************************************************************

class Estanquero implements Runnable{
	public Thread thr;
	private Estanco estanco;
	
	public Estanquero(Estanco estanco){
		this.estanco = estanco;
		thr = new Thread(this, "Estanquero ");
	}
	
	public void run(){
		try{
			int ingrediente;
			while(true){
				ingrediente = (int) (Math.random() * 3.0);
				estanco.ponerIngrediente(ingrediente);
				estanco.esperarRecogidaIngrediente();
			}
		}catch(Exception e){
			System.err.println("\033[31m Error en Estanquero: " + e);
		}
	}
}

// ****************************************************************************

class Fumador implements Runnable{
	private int miIngrediente;
	public Thread thr;
	private Estanco estanco;
	
	public Fumador(int p_miIngrediente, Estanco estanco){
		miIngrediente = p_miIngrediente;
		this.estanco = estanco;
		thr = new Thread(this, "Fumador " + miIngrediente);
	}
	
	public void run(){
		try{
			while(true){
				estanco.obtenerIngrediente(miIngrediente);
				aux.dormir_max(2000);
			}
		}catch(Exception e){
			System.err.println("\033[31m Error en Fumador: " + e + " \033[0m");
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
			System.err.println("\033[31m Error al dormir la hebra \033[0m");
		}
	}
}

// ****************************************************************************

class MainFumadores{
	public static void main(String[] args){
		Estanco estanco = new Estanco();
		Estanquero estanquero = new Estanquero(estanco);
		Fumador[] fumadores = new Fumador[3];

		for(int i = 0; i < 3; i++)
			fumadores[i]= new Fumador(i, estanco);

		estanquero.thr.start();
		for(int i = 0; i < 3; i++)
			fumadores[i].thr.start();
	}
}
