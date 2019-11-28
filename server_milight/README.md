To make sure the server runs correctly do the following:
1)npm install
2)In .env file:
  a) MILIGHT_IP = Set the milight IP by connecting to the milight hub through wifi and using ifconfig tool
  b) PRU_FILE = Give absolute path of the file you want PRU to read.
  c) SCREENSHOT_FILE = Give absolute path of the file that recieveScreenshot is writing too. Which is an environment variable RGB_SCREENCAPTURE_FILE.

  NOTE: if PRU_FILE and SCREENSHOT_FILE is not created, the server will not work
