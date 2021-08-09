
#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include<System.h>

#include<time.h>

using namespace std;

// From http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

int main(int argc, char **argv)
{
	string vocabPath = "./Vocabulary/ORBvoc.txt";
	string settingsPath = "./Examples/Monocular/LifeCamStudio.yaml";
	if (argc == 1)
	{

	}
	else if (argc == 2)
	{
		vocabPath = argv[1];
	}
	else if (argc == 3)
	{
		vocabPath = argv[1];
		settingsPath = argv[2];
	}
    else
    {
        cerr << endl << "Usage: mono_webcam path_to_vocabulary path_to_settings" << endl;
        return 1;
    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM3::System SLAM(vocabPath, settingsPath,ORB_SLAM3::System::MONOCULAR,true);

    cout << endl << "-------" << endl;
    cout << "Start processing sequence ..." << endl;

	cv::VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);

	// From http://stackoverflow.com/questions/19555121/how-to-get-current-timestamp-in-milliseconds-since-1970-just-the-way-java-gets
	int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


	// Main loop
	cv::Mat im;
	cv::Mat Tcw;
    while (true)
    {
		cap.read(im);

		int64_t curNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		// Pass the image to the SLAM system
		Tcw = SLAM.TrackMonocular(im, (double)curNow / 1000.0);

		/* This can write each image with its position to a file if you want
		if (!Tcw.empty())
		{
			cv::Mat Rwc = Tcw.rowRange(0, 3).colRange(0, 3).t();
			cv::Mat twc = -Rwc*Tcw.rowRange(0, 3).col(3);
			std::ostringstream stream;
			//stream << "imgs/" << Rwc.at<float>(0, 0) << " " << Rwc.at<float>(0, 1) << " " << Rwc.at<float>(0, 2) << " " << twc.at<float>(0) << " " <<
			//	Rwc.at<float>(1, 0) << " " << Rwc.at<float>(1, 1) << " " << Rwc.at<float>(1, 2) << " " << twc.at<float>(1) << " " <<
				//Rwc.at<float>(2, 0) << " " << Rwc.at<float>(2, 1) << " " << Rwc.at<float>(2, 2) << " " << twc.at<float>(2) << ".jpg";
			stream << "imgs/" << curNow << ".jpg";
			string fileName = stream.str();
			cv::imwrite(fileName, im);
		}
		*/

		// This will make a third window with the color images, you need to click on this then press any key to quit
		cv::imshow("Image", im);


		if (cv::waitKey(1) >= 0)
			break;
    }

    // Stop all threads
    SLAM.Shutdown();
	cap.release();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");

    return 0;
}