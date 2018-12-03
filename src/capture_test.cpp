#include <opencv2/opencv.hpp>
#include <exiv2/exiv2.hpp>
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
bool notDone = true;
unsigned int itemCount = 0;
string frontFile = "front.jpg";
string backFile = "back.jpg";

/* For future use.  Need to transfer thread handler for Qt
void spinVideo() {

} */

void getUserInput() 
{

   while(notDone)
   {

      itemCount++;
      frontFile = "front" + to_string(itemCount) + ".jpeg";
      backFile = "back" + to_string(itemCount) + ".jpeg";

      while(true)
      {

         printf("Item %d: Position front side and press enter: ",itemCount);
         getline(cin, in);

         if (in.empty())
         {

            printf("Capturing front image\n");
            imwrite(frontFile, frame);
            break;

         }
         if (in == "c")
         {

            printf("Exiting user input thread\n");
            notDone = false;
            return;

         }

      }

      while(true)
      {

         printf("Item %d: Position back side and press enter: ",itemCount);
         getline(cin, in);

         if (in.empty())
         {

            printf("Capturing back image\n");
            imwrite(backFile, frame);
            break;

         }
         if (in == "c")
         {

            printf("Exiting user input thread\n");
            notDone = false;
            return;

         }

      }

      while(true)
      {

         // Use generic categories for now
         printf("Item %d: Enter category (t, j, s): ",itemCount);
         getline(cin, in);

         if (in == "t" || in == "j" || in == "s")
         {

            type = in;
            printf("Type is %s\n",type.c_str());
            break;

         }
         if ( in == "c" )
         {

            printf("Exiting user input thread\n");
            notDone = false;
            return;

         }
   
      }

      // Exif tagging
      printf("Tagging images\n");
      Exiv2::Image::AutoPtr front_image = Exiv2::ImageFactory::open(frontFile);
      Exiv2::ExifData ed_front;
      ed_front["Exif.Image.ImageDescription"] = "Front";
      // Use standard exif tag to store the item count for now
      // Neet to create a custom MakerNote
      ed_front["Exif.Image.Make"] = to_string(itemCount);
      // Use standard exif tag to store the item type for now
      // Neet to create a custom MakerNote
      ed_front["Exif.Image.Model"] = type;
      front_image->setExifData(ed_front);
      front_image->writeMetadata();

      Exiv2::Image::AutoPtr back_image = Exiv2::ImageFactory::open(backFile);
      Exiv2::ExifData ed_back;
      ed_back["Exif.Image.ImageDescription"] = "Back";
      // Use standard exif tag to store the item count for now
      // Neet to create a custom MakerNote
      ed_back["Exif.Image.Make"] = to_string(itemCount);
      // Use standard exif tag to store the item count for now
      // Neet to create a custom MakerNote
      ed_back["Exif.Image.Model"] = type;
      back_image->setExifData(ed_back);
      back_image->writeMetadata();

      // Use generic categories for now
      printf("Continue to next item (y/n)? ");
      getline(cin, in);

      if (in == "y")
      {

         // Continue

      }
      else
      {

         printf("Exiting user input thread\n");
         printf("Enter c in the camera window to close program.\n");
         notDone = false;

      }

   }

   return;

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
