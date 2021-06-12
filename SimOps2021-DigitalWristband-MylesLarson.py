# This program uses the papirus library to update the display
# with one of 4 sets of petient data. There is also an option
# to clear the screen. The code can be directly editted to change
# the patient data or it is possible to create a function
# to take user input for the patient data.

# Set up our library and GPIO.
from papirus import PapirusComposite
import RPi.GPIO as GPIO
from time import sleep

# Set up part of the library to allow a mix of text and bitmap images.
text = PapirusComposite(False)
# Set up variables for the GPIO pins used by the buttons on the Papirus display.
SW1 = 26
SW2 = 19
SW3 = 20
SW4 = 16
SW5 = 21

# Finish our button and GPIO set up by assigning the pins as inputs.
GPIO.setmode(GPIO.BCM)
GPIO.setup(SW1, GPIO.IN)
GPIO.setup(SW2, GPIO.IN)
GPIO.setup(SW3, GPIO.IN)
GPIO.setup(SW4, GPIO.IN)
GPIO.setup(SW5, GPIO.IN)

# Define a function to handle writing our data to the display and some info to our computer screen.
def writeStuff(toPrint, toWrite):
    # Output to the computer screen which patient we are pushing to the Papirus.
    print(toPrint)
    # Get ready to write the patient data to the Papirus.
    text.AddText(toWrite, 0, 0, Id="pID")
    # Get a barcode image for the bottom of the Papirus.
    # Barcode formatted in Code 128 can only be about 7 characters to still be scanned.
    # This is a limitation of the resolution of the Papirus.
    # This example uses one image from a specific file location for any button push.
    # Another variable added to this function could be used to select a different image for each button.
    text.AddImg("/home/pi/Pictures/ABC123X.xbm", 0,77,(200,50), Id="barcode")
    # Actually push the data tot he Papirus.
    text.WriteAll()
    # Clear the text data we used to prepare for the next button push.
    text.RemoveText("pID")
    # Clear the image data we used to prepare for the next button push.
    text.RemoveImg("barcode")

# An indefinite loop to check for button presses and update the display.
while True:
    # A set of If statements checking which button was pressed and updating
    # our Papirus with some patient data.
    if GPIO.input(SW1) == False:
        writeStuff("Button1: Thomas Petty", "Patient Name:\nPetty, Thomas\nDOB: 01/30/1980\nSex:M NKDA")
    if GPIO.input(SW2) == False:
        writeStuff("BUTTON2: Maggie Smith", "Patient Name:\nSmith, Maggie\nDOB: 02/10/1985\nSex:F NKDA")
    if GPIO.input(SW3) == False:
        writeStuff("BUTTON3: Johnathan Marks", "Patient Name:\nMarks, Johnathan\nDOB: 03/20/1990\nSex:M  NKDA")
    if GPIO.input(SW4) == False:
        writeStuff("BUTTON4: Susan Dink", "Patient Name:nDink, Susan\nDOB: 05/18/1970\nSex:F  NKDA")
    if GPIO.input(SW5) == False:
        # Output to our computer screen what is happening.
        print("BUTTON5: Clear Screen")
        # Clear the Papirus display using a library function.
        text.Clear()
    # Output to our computer screen that we recognized the button was released.
    else:
        print("released")
    # Wait a moment before we loop again.
    sleep(1)