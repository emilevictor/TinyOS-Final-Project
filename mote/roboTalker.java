import net.tinyos.message.*;
import net.tinyos.util.*;
import java.io.*;

public class roboTalker implements MessageListener
{

	MoteIF mote;

	void run() {
		mote = new MoteIF(PrintStreamMessenger.err);
		mote.registerListener(new RobotMessaging(),this);

	}


    public synchronized void messageReceived(int dest_addr, 
            Message msg) {
    	System.out.println("Message received");

	    if (msg instanceof RobotMessaging) {
	        RobotMessaging rm = (RobotMessaging)msg;

	        /* Update interval and mote data */
	        //periodUpdate(omsg.get_version(), omsg.get_interval());
	        System.out.println(rm.toString());

	        //data.update(omsg.get_id(), omsg.get_count(), omsg.get_readings());

	        /* Inform the GUI that new data showed up */
	        //window.newData();
	    }
    }

	void sendMessage()
	{
		RobotMessaging rm = new RobotMessaging();

		/*try {
			rm.send(MoteIF.TOS_BCAST_ADDR, rm);
		} catch (IOException e) {
			System.out.println("Cannot send message");
		}*/
	}



	public static void main(String[] args)
	{
		roboTalker me = new roboTalker();
		me.run();
	}

}