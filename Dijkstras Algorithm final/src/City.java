

public class City {
    String name;
    Connection[] neighbors;
    int ind = 0;
    Integer id;
    

    public City(String name, Integer id) {
        this.name = name;
        this.neighbors = new Connection[20];
        this.id = id;
        
    }

    public void add(City next, int distance) {

        Connection hold = new Connection(next,distance);
        
        this.neighbors[ind++] = hold;
        
    }
}