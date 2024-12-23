public class Path {

        City city;
        City prev;
        Integer dist;
        Integer index;

        public Path(City city, City prev, Integer dist) {
            this.city = city;
            this.prev = prev;
            this.dist = dist;
            this.index = city.id;

        }

        


}