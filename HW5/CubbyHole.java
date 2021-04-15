public class CubbyHole {
   private int contents;               //Shared data
   private boolean available = false;  //Is something available?

   public synchronized int get() {        //synchronized IS the key....
      while (available == false) {
         try {
            wait();  //Waits for another thread to call notify();
         } catch (InterruptedException e) { } //We have this because a call can be made to cause an interrupt
      }
      available = false;
      notifyAll();  //the signal
      return contents;
   }

   public synchronized void put(int value) {
      while (available == true) {
           try {
              wait();
           } catch (InterruptedException e) { }
      }
     contents = value;
     available = true;
     notifyAll();
   }
}
