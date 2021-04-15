public class DiningThread extends Thread{

private Barrier barrier;
private String  task; //e.g. "Task A"
private int     id; 

public DiningThread (Barrier b, String work, int threadid) {
barrier = b;
task    = work;
id      = threadid;

}


public void run ( ) {




}


public static void main (String [] args)throws InterruptedException {

Barrier b = new Barrier (4); //4 threads must be synchronized

// create DiningThread objects



// start them


// join them


} 
}

