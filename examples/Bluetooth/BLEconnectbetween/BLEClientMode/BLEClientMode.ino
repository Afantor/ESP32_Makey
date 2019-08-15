/*
 * Program to operate ESP32 in client mode and use other esp32 board as proximity switch
 * Program by: Afantor
 * Dated: 15-08-2019
 * Website: www.circuitdigest.com 
 * Reference: https://github.com/nkolban/esp32-snippets 
 * //NOTE: The My_BLE_Address, serviceUUID and charUUID should be changed based on the BLe server you are using 
 */
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

// The remote service we wish to connect to.
static BLEUUID 	serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID  charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
//Hardware Bluetooth MAC of other esp32 board, will vary for every band obtained through nRF connect application 
String My_BLE_Address = "B$:E6:2D:98:15:0F"; 

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;

static BLERemoteCharacteristic* pRemoteCharacteristic;

BLEScan* pBLEScan; //Name the scanning device as pBLEScan
BLEScanResults foundDevices;

static BLEAddress *Server_BLE_Address;
String Scaned_BLE_Address;

boolean paired = false; //boolean variable to togge light

// this gets called when we receive a message
// Note that the server has to call notify to trigger this
// This function just prints the received message
static void notifyCallback(
	BLERemoteCharacteristic* pBLERemoteCharacteristic,
	uint8_t* pData,
	size_t length,
	bool isNotify)
{
	Serial.print("Notify callback for characteristic ");
	Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
	Serial.print("Data: ");
	Serial.println(length);
	// Print out the message that we received
	for (int i = 0; i < length; i++)
		Serial.print((char)pData[i]);
	Serial.println();
}

bool connectToServer(BLEAddress pAddress)
{
	Serial.print("Forming a connection to ");
	Serial.println(pAddress.toString().c_str());

	BLEClient*  pClient = BLEDevice::createClient();
	Serial.println(" - Created client");

	// Connect to the remove BLE Server.
	pClient->connect(pAddress);
	Serial.println(" - Connected to server");

	// Obtain a reference to the service we are after in the remote BLE server.
	BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
	if (pRemoteService == nullptr) {
		Serial.print("Failed to find our service UUID: ");
		Serial.println(serviceUUID.toString().c_str());
		return false;
	}
	Serial.println(" - Found our service");


	// Obtain a reference to the characteristic in the service of the remote BLE server.
	pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
	if (pRemoteCharacteristic == nullptr) {
		Serial.print("Failed to find our characteristic UUID: ");
		Serial.println(charUUID.toString().c_str());
		return false;
	}
	Serial.println(" - Found our characteristic");

	// Bind the notifyCallback method to the characteristic
	pRemoteCharacteristic->registerForNotify(notifyCallback);
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks 
{
	/**
	  * Called for each advertising BLE server.
	  */
	void onResult(BLEAdvertisedDevice advertisedDevice) {
		Serial.print("BLE Advertised Device found: ");
		Serial.println(advertisedDevice.toString().c_str());

		Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());
      
      	Scaned_BLE_Address = Server_BLE_Address->toString().c_str();

		// We have found a device, let us now see if it contains the service we are looking for.
		if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

			// 
			Serial.print("Found our device!  address: ");
			advertisedDevice.getScan()->stop();

			pServerAddress = new BLEAddress(advertisedDevice.getAddress());
			doConnect = true;

		} // Found our server
	} // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() 
{
	Serial.begin(115200);
	Serial.println("Starting ESP32 Arduino BLE Client application...");
	BLEDevice::init("");

	// Retrieve a Scanner and set the callback we want to use to be informed when we
	// have detected a new device.  Specify that we want active scanning and start the
	// scan to run for 30 seconds.
	BLEScan* pBLEScan = BLEDevice::getScan();//create new scan
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());//Call the class that is defined above 
	pBLEScan->setActiveScan(true);//active scan uses more power, but get results faster
	pBLEScan->start(30);
	pinMode (2,OUTPUT); //Declare the in-built LED pin as output
} // End of setup.


// This is the Arduino main loop function.
void loop() 
{

	// If the flag "doConnect" is true then we have scanned for and found the desired
	// BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
	// connected we set the connected flag to be true.
	// if (doConnect == true) {
	// 	if (connectToServer(*pServerAddress)) {
	// 		Serial.println("We are now connected to the BLE Server.");
	// 		connected = true;
	// 	}
	// 	else {
	// 		Serial.println("We have failed to connect to the server; there is nothing more we will do.");
	// 	}
	// 	doConnect = false;
	// }

	// delay(50); // do a delay to keep things ticking over
	foundDevices = pBLEScan->start(3); //Scan for 3 seconds to find the Fitness band 

  	while (foundDevices.getCount() >= 1)
  	{
  	  	if (Scaned_BLE_Address == My_BLE_Address && paired == false)
  	  	{
  	    	Serial.println("Found Device :-)... connecting to Server as client");
  	     	if (connectToServer(*Server_BLE_Address))
  	    	{
  	    		paired = true;
  	    		Serial.println("********************LED turned ON************************");
  	    		digitalWrite (2,HIGH);
  	    		break;
  	    	}
  	    	else
  	    	{
  	    		Serial.println("Pairing failed");
  	    		break;
  	    	}
  	  	}
  	  
  	  	if (Scaned_BLE_Address == My_BLE_Address && paired == true)
  	  	{
  	    	Serial.println("Our device went out of range");
  	    	paired = false;
  	    	Serial.println("********************LED OOOFFFFF************************");
  	    	digitalWrite (2,LOW);
  	    	ESP.restart();
  	    	break;
  	  	}
  	  	else
  	  	{
  	  		Serial.println("We have some other BLe device in range");
  	  		break;
  	  	}
  	} 
}