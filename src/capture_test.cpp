#include<opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <string>

using namespace cv;
using namespace std;

// Create globals for quick prototyping
// Should use proper OO concepts in future
string type;
string in;
VideoCapture usbcam;
Mat frame;

/* For future use.  Need to transfer thread handler for Qt
void spinVideo() {

} */

void getUserInput() 
{

   while(true)
   {

      printf("Position front side and press enter: ");
      getline(cin, in);

      if (in.empty())
      {

         printf("Capturing front image\n");
         imwrite("front.jpg", frame);
         break;

      }
      if (in == "c")
      {

         printf("Exiting user input thread\n");
         return;

      }

   }

   while(true)
   {

      printf("Position back side and press enter: ");
      getline(cin, in);

      if (in.empty())
      {

         printf("Capturing back image\n");
         imwrite("back.jpg", frame);
         break;

      }
      if (in == "c")
      {

         printf("Exiting user input thread\n");
         return;

      }

   }

   while(true)
   {

      // Use generic categories for now
      printf("Enter category (t, j, s): ");
      getline(cin, in);

      if (in == "t" || in == "j" || in == "s")
      {

         type = in;
         printf("Type is %s\n",type.c_str());
         printf("Enter c in the camera window to close program.\n");
         return;

      }
      if ( in == "c" )
      {

         printf("Exiting user input thread\n");
         return;

      }
   
   }

}

int main()
{

   // For future use
   //std::thread videoThread(spinVideo);

   // This thread is okay for now.
   // Need to implement improved thread handling.
   std::thread inputThread(getUserInput);
   inputThread.detach();

   char key;

   // Tim's built-in webcam is /dev/video0, usb camera is /dev/video1
   usbcam.open(1);

   while(true)
   {

      usbcam >> frame;
      imshow("Cam",frame);

      key = waitKey(60);

      if(key=='c') 
      {

         printf("Program termination initiated through the camera window.\n");
         break;

      }

   }

   printf("Releasing usbcam\n");
   usbcam.release();
   printf("usbcam released\n");

   return 0;

}
