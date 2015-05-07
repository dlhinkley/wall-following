
all: build load

build:
	javac Status.java
	javac -classpath . WallFollowing.java
	javac -classpath .:/usr/share/java/RXTXcomm.jar SerialTest.java

run:

	java -Djava.library.path=/usr/lib/jni -classpath .:/usr/share/java/RXTXcomm.jar SerialTest



