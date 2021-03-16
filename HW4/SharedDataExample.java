public class SharedDataExample {

private int [] myValue; //shared data 

public SharedDataExample ( ) {


       myValue = new int [3];

       // Create 3 threads
      ExampleThread [] thr = new ExampleThread[3];  
     
      for (int i = 0; i < 3; i++ ) {
          thr[i] = new ExampleThread (i);
      }

      // Start the 3 threads
      for (int i = 0; i < 3; i++ ) {
          thr[i].start();
      }


     // Block until all 3 are finished
     try {

     for (int i = 0; i < 3; i++ ) {
         thr[i].join();
     }

     }
     catch (InterruptedException e) {
          System.out.println (e);
     }
 
}
private class ExampleThread extends Thread{
        private int arg;        
        public ExampleThread (int parm) {
        arg = parm; 
        }


       public void run() {
                   myValue[arg] = arg;
                   System.out.println ("myValue["+arg+"] = "+myValue[arg]);
       }//end run
}//end inner class

public static void main( String []args) {

       SharedDataExample s = new SharedDataExample();
}//endmain 
}//end SharedDataExample
