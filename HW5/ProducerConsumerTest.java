public class ProducerConsumerTest {
   public static void main(String[] args) {
      CubbyHole c = new CubbyHole();    //1 monitor
      Producer p1 = new Producer(c, 1); //1 producer
      Consumer c1 = new Consumer(c, 1); //1 consumer

      p1.start(); //Call run...
      c1.start(); //Call run...
  }
}




