#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	    //create the socket and bind to port 11999
	ofSetLogLevel(OF_LOG_VERBOSE);

	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);

	//load config file
	    //load config.xml
    if(!config.loadFile(ofToDataPath("config.xml")))
    {
        ofLog() << "Failed to load config.xml";
    }

	int numPix = config.getValue("NUMLIGHTS", 20);
	ofLog() << "number of pixels to run: " << numPix;

	if(sm16716.init(numPix))
	{
		ofLog() << "main: init of sm16716 successful";
	} 
    
    blankCounter = 0;


	

}

//--------------------------------------------------------------
void ofApp::update(){
	



	char udpMessage[200000];
	
	//get the UDP mesage
	int result = udpConnection.Receive(udpMessage,200000);
	
	//if there is a message
	if(result > 0)
	{
		blankCounter = 0;
		//get the message
		string message=udpMessage;
		
		//if the message is not blank, start parsing
		if(message!="")
		{
			
			//set up variables for the channel, red, green, blue values
			uint32_t chan, r,g,b;

			//each pixelPacket is separated by "[/p]"
			vector<string> pixelPackets = ofSplitString(message,"[/p]");

			//go through each pixelPacket and parse it
			for(unsigned int i=0;i<pixelPackets.size();i++)
			{
				
				//we should have four pixel values for each packet: chan, r, g, b
				vector<string> pixelValues = ofSplitString(pixelPackets[i],"|");
				if( pixelValues.size() == 4 )
				{
					
					chan = atol(pixelValues[0].c_str());	//first value is the channel, or pixel address
					r = atol(pixelValues[1].c_str());	//second value is red
					g = atol(pixelValues[2].c_str());	//third value is green
					b = atol(pixelValues[3].c_str());	//fourth value is blue
					
					if(chan == 1) ofLog() << "GOT ONE!";

					// val = curvedMap(val);
					//set the pixel values 
					sm16716.set_pixel_rgb(chan, r, g, b );
				}
			}
 			// ofLogVerbose() << "FrameRate: " << ofGetFrameRate();
		}
	}
	else if (result < 0)
	{
		//ofLog() << "nothing there";
		blankCounter ++;
	}

	//once all of the pixels are set, send them out!
	sm16716.show();


	//if we don't receive data for 1000 counts, clear everything
	if (blankCounter == 1000)
	{
		sm16716.clear();
		blankCounter = 0;

	}


	ofLogVerbose() << ofGetFrameRate();
	
}
		

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

