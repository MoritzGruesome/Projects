

public class Bench {
    public static void main(String[] args) throws Exception {
        
        Map map = new Map("europe.csv");
        

        long  t0 = System.nanoTime();
        Integer dist = map.shortest("London", "Stockholm");
        long time = (System.nanoTime() - t0)/1_000;
        // print shortest travel time and search time in microseconds
        System.out.println("shortest: " + dist + " min (" + time + " us)");
    
        
    }


    

    

        


    
}
