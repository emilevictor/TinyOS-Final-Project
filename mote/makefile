all: RobotMessaging.java RobotMessaging.class Constants.java Constants.class roboTalker.class roboTalker.jar


RobotMessaging.java: packet.h
	mig -target=null -java-classname=RobotMessaging java packet.h packet -o RobotMessaging.java 

Constants.java: packet.h
	ncg -target=null -java-classname=Constants java packet.h NREADINGS DEFAULT_INTERVAL -o Constants.java 

Constants.class RobotMessaging.class roboTalker.class:
	javac *.java

roboTalker.jar: roboTalker.class
	jar cf roboTalker.jar *.class

clean:
	rm -f *.class $(GEN)

veryclean: clean
	rm roboTalker.jar
