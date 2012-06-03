import net.tinyos.message.*;
import net.tinyos.util.*;
import java.io.*;
import java.util.Arrays;

//Type "make"
//Then run serialforwarder: java net.tinyos.sf.SerialForwarder -comm serial@/dev/ttyUSB1:iris


public class roboSender implements MessageListener
{

	MoteIF mote;

	void run() {
		//System.out.println("Starting motes");
		mote = new MoteIF(PrintStreamMessenger.err);
		//mote.registerListener(new RobotMessaging(),this);
		//System.out.println("Motes started");

		//Forever, take input from stdin.
		System.out.println("I am alive!");
		while(true)
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
	    	String command = null;
			try {
				command = br.readLine();
			} catch (IOException e) {
				System.out.println("Error!");
				System.exit(1);
			}

			command = command.trim();
			String[] splitCommand = command.split(" ");
			System.out.println("Received string");	
			if (splitCommand.length >= 8)
			{
				Short mode = Short.valueOf(splitCommand[0]);
				Short cmd = Short.valueOf(splitCommand[1]);
				Short cmd_duration = Short.valueOf(splitCommand[2]);
				Short ack = Short.valueOf(splitCommand[3]);
				Short req_info = Short.valueOf(splitCommand[4]);
				Short current_x = Short.valueOf(splitCommand[5]);
				Short current_y = Short.valueOf(splitCommand[6]);
				Short hit_wall = Short.valueOf(splitCommand[7]);
				Short globalTime = Short.valueOf(splitCommand[8]);

				sendMessage(mode,cmd,cmd_duration,ack,req_info,current_x,current_y,hit_wall,globalTime);
			} else {
				System.out.println("Message not sent, not enough arguments provided.");
			}
		}



	}


    public synchronized void messageReceived(int dest_addr, 
            Message msg) {

    	//Do nothing.

    }

	public void sendMessage(Short mode, Short cmd, Short cmd_duration,
		Short ack, Short req_info, Short current_x, Short current_y,
		Short hit_wall, Short globalTime)
	{
		RobotMessaging rm = new RobotMessaging();
		rm.set_mode(mode);
		rm.set_cmd(cmd);
		rm.set_cmd_duration(cmd_duration);
		rm.set_ack(ack);
		rm.set_req_info(req_info);
		rm.set_current_x(current_x);
		rm.set_current_y(current_y);
		rm.set_hit_wall(hit_wall);
		rm.set_globalTime(globalTime);
	    try {
	        mote.send(MoteIF.TOS_BCAST_ADDR, rm);
	    } catch (IOException e) {
	        System.out.println("Error sending message");
	        return;
	    }
	    System.out.println("Message sent");

	}



	public static void main(String[] args)
	{
		roboSender me = new roboSender();
		me.run();
		System.out.println("running");
	}

}