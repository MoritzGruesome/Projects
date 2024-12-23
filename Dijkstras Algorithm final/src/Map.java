import java.io.BufferedReader;
import java.io.FileReader;

public class Map {

    // Hash function
    private Integer hash(String name) {
        int hash = 0;
        for (int i = 0; i < name.length(); i++) {
            hash = (hash * 31 % mod) + name.charAt(i);
        }
        return hash % mod;
    }

    private City[] cities;
    private final int mod = 541;
    int collisions = 0;
    int index = 0;

    public Map(String file) {
        
        //read the start and end stations including distance from file
        cities = new City[mod];
        try (BufferedReader br = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = br.readLine()) != null) {
                
                String[] res = line.split(",");
                
                City c1 = lookup(res[0]);
                City c2 = lookup(res[1]);
                
                int distance = Integer.parseInt(res[2]);
                c1.add(c2, distance);
                c2.add(c1, distance);
                
            }
        } catch (Exception e) {
            System.out.println(" file " + file + " not found or corrupt");
        }
    }

    // Look-up for the hashed table
    public City lookup(String name) {
        int hash = hash(name);

        if (cities[hash] != null) {

            if (cities[hash].name.equals(name)) {
                return cities[hash];
            } else {

                int h = hash + 1;
                while (h < mod) {
                    if (cities[h] != null) {
                        if (cities[h].name.equals(name))
                            return cities[h];
                    }
                    h++;
                }

                h = hash + 1;
                while (cities[h] != null) {
                    h++;
                    collisions++;
                }
                cities[h] = new City(name,index++);
                collisions++;
                
                
                return cities[h];
            }
        } else {
            cities[hash] = new City(name,index++);
            return cities[hash];
        }
    }

    public Integer shortest(String fro, String t) {

        City from = lookup(fro);
        City to = lookup(t);
        Queue queue = new Queue(); // setup queue
        Path[] done = new Path[200];
        Boolean searching = true;
       
        
        queue.queue(new Path(from, null, 0)); // first path

        while(searching) { // main while loop
            Path cNode = queue.dequeue(); // pop queue
            if (cNode.city == to) { // if we arrived at desired node
                
                return cNode.dist; // return
                
                
            }
            if(done[cNode.index] == null) { // place city in processed set if empty
            done[cNode.index] = cNode; // add node to done
            }
            for(int i = 0; i < cNode.city.ind;i++) { 
            // for all connections from the node
                Connection dConn = cNode.city.neighbors[i];
                
                if (dConn == null) {
                    continue;
                }
                if (dConn.city != cNode.city) { // if we dont go backwards
                     if(queue.append(dConn.city.name, dConn.distance+cNode.dist, cNode.city)) {
                        
                        continue;
                     }
                     else {
                        queue.queue(new Path(dConn.city,cNode.city,dConn.distance+cNode.dist));
                     }
                }
            }
        }
        return null;
    }

}
