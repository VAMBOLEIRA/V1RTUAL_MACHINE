int x;
int y;
int z;

func main() {
    x = 5;
    y = 10;
    z = 0;
    
    if (x) {
        z = x + y;
    }
    
    if (z) {
        z = z * 2;
    }
    
    return z;
}