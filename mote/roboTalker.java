import net.tinyos.message.*;
import net.tinyos.util.*;
import java.io.*;


//Type "make"
//Then run serialforwarder: java net.tinyos.sf.SerialForwarder -comm serial@/dev/ttyUSB1:iris
////java -cp /home/user/local/src/tinyos-2.x/support/sdk/java/tinyos.jar:/home/user/local/src/finalQtProject/finalProject/Final/mote roboTalker


public class roboTalker implements MessageListener
{

	MoteIF mote;

	void run() {
		System.out.println("Starting motes");
		mote = new MoteIF(PrintStreamMessenger.err);
		mote.registerListener(new RobotMessaging(),this);
	}

    public synchronized void messageReceived(int dest_addr, 
            Message msg) {

	    if (msg instanceof RobotMessaging) {
	        RobotMessaging rm = (RobotMessaging)msg;

	        /* Update interval and mote data */
	        //periodUpdate(omsg.get_version(), omsg.get_interval());
	        System.out.println(rm.get_mode() + " " + rm.get_cmd() + " " + rm.get_cmd_duration() + " " + rm.get_ack() + " " + rm.get_req_info() + " " + rm.get_current_x() + " " + rm.get_current_y() + " " + rm.get_hit_wall());
	        //data.update(omsg.get_id(), omsg.get_count(), omsg.get_readings());

	        /* Inform the GUI that new data showed up */
	        //window.newData();
	    }
    }

	public void sendMessage()
	{
		RobotMessaging rm = new RobotMessaging();
		rm.set_mode((short)1);
		rm.set_cmd((short)0);
		rm.set_cmd_duration((short)1000);
		rm.set_ack((short)0);
		rm.set_req_info((short)0);
		rm.set_current_x((short)11);
		rm.set_current_y((short)12);
		rm.set_hit_wall((short)0);
	    try {
	        mote.send(MoteIF.TOS_BCAST_ADDR, rm);
	    } catch (IOException e) {
	        System.out.println("Error sending data");
	    }


	}



	public static void main(String[] args)
	{
		roboTalker me = new roboTalker();
		me.run();
		System.out.println("running");
	}

}