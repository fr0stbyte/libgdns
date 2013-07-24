import java.net.*;

class Test { 
    public static void main(String args[]) throws UnknownHostException{
        String name = new String("www.gogl.com");
        System.out.println("[test.java] looking up " + name);
        InetAddress Address = InetAddress.getByName(name);
    }
}