public class Queue {
    
    // each bag contains an item and priority
    // the bag is added to the queue
    private class bag {
        Path item;
        int pr;

        private bag(Path item) {
            this.item = item;
            this.pr = item.dist;
        }

        private bag() {
            this.item = null;
            this.pr = 0;
        }
    }
    
    bag[] heap = new bag[200];
    
    int root = 0;
    int k;

    public Queue() {
        this.k = -1;
        for(int i = 0; i < heap.length; i++) {
            heap[i] = new bag();
        }
    }
    // bubble function for priority queue 
    public void bubble(int k) {
        boolean even;
        if(k == root)
            return;
        if(k%2 == 0) { // determine parity
            even = true;
            if (heap[k].pr >= heap[(k-2)/2].pr)
            return;
        }
        else {
            even = false;
            if (heap[k].pr >= heap[(k-1)/2].pr)
            return;
        }
        if (even) { // swap right node with parent
            bag carry = heap[(k-2)/2];
            heap[(k-2)/2] = heap[k];
            heap[k] = carry;
            bubble((k-2)/2);
        }
        else {      
            bag carry = heap[(k-1)/2]; // swap left node with parent
            heap[(k-1)/2] = heap[k];
            heap[k] = carry;
            bubble((k-1)/2);
        }
    }

    public void sink(int node) {
        if(heap[node*2+1].pr == 0 && heap[node*2+2].pr == 0)
            return;

        if (heap[node].pr >= heap[node*2+1].pr & (heap[node*2+1].pr > 0)) { // if left node smaller
            if (heap[node*2+1].pr >= heap[node*2+2].pr & (heap[node*2+2].pr > 0)) { 
                // if left node smaller than right 
                bag carry = heap[node]; // swap left
                heap[node] = heap[node*2+2];
                heap[node*2+2] = carry;
                sink(node*2+2);
            }
            else { // if else swap right
                bag carry = heap[node];
                heap[node] = heap[node*2+1];
                heap[node*2+1] = carry;
                sink(node*2+1);
            }
        }
        if (heap[node].pr >= heap[node*2+2].pr & (heap[node*2+2].pr > 0)) { // if left node smaller
            if (heap[node*2+1].pr <= heap[node*2+2].pr & (heap[node*2+1].pr > 0)) { 
                // if left node smaller than right 
                bag carry = heap[node]; // swap left
                heap[node] = heap[node*2+1];
                heap[node*2+1] = carry;
                sink(node*2+1);
            }
            else { // if else swap right
                bag carry = heap[node];
                heap[node] = heap[node*2+2];
                heap[node*2+2] = carry;
                sink(node*2+2);
            }    
        }
    }

    public void queue(Path priority) {
        heap[++k] = new bag(priority);
            bubble(k);
    }

    public Path dequeue() {
        Path hp = heap[root].item;
        heap[root] = heap[k];
        heap[k--] = new bag();
        sink(root);
        return hp;
    }

    public Boolean append(String target, int replacement, City prev) {

        for(int i = 0; i <= k; i++) {
            if(target.equals(heap[i].item.city.name)) {
                if(replacement < heap[i].pr) {
                heap[i].item.dist = replacement;
                heap[i].item.prev = prev;
                heap[i].pr = replacement;
                bubble(i);
            }
                return true;
            }
        }
        return false;

        
    

}
}
