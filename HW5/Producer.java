public class Producer extends Thread {
   private CubbyHole cubbyhole;  //This will be used by both producer and consumer...same monitor
   private int number;

   public Producer(CubbyHole c, int number) {
      cubbyhole = c;
      this.number = number;
   }

   public void run() {  //Since we extend Thread, we have a method called run() which is the thread method
     for (int i = 0; i < 10; i++) {
       cubbyhole.put(i);
       System.out.println("Producer #" + this.number + " put: " + i);
        try {
            sleep((int)(Math.random() * 100));
        } catch (InterruptedException e) { }
     }
  }
}

