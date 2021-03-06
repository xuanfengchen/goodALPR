#include "../include/Pipeline.h"
#include <vector>
#include "../include/tinyxml.h"
#include "tinystr.h"
#include "boost/filesystem.hpp"
#include "../src/util.h"

std::vector<std::string> CH_PLATE_CODE{ "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "³", "ԥ", "��", "��", "��", "��",
                                       "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A",
                                      "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
                                   "Y", "Z","��","ѧ","ʹ","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��"};



using namespace std;
namespace fs = boost::filesystem;

template<class T>
static unsigned int levenshtein_distance(const T &s1, const T &s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2 + 1), prevCol(len2 + 1);

	for (unsigned int i = 0; i < prevCol.size(); i++) prevCol[i] = i;
	for (unsigned int i = 0; i < len1; i++) {
		col[0] = i + 1;
		for (unsigned int j = 0; j < len2; j++)
			col[j + 1] = min(
			min(prevCol[1 + j] + 1, col[j] + 1),
			prevCol[j] + (s1[i] == s2[j] ? 0 : 1));
		col.swap(prevCol);
	}
	return prevCol[len2];
}




/*void TEST_ACC(){

	pr::PipelinePR prc("model/cascade.xml",
		"model/HorizonalFinemapping.prototxt", "model/HorizonalFinemapping.caffemodel",
		"model/Segmentation.prototxt", "model/Segmentation.caffemodel",
		"model/CharacterRecognization.prototxt", "model/CharacterRecognization.caffemodel",
		"model/SegmentationFree.prototxt", "model/SegmentationFree.caffemodel"
		);

	ifstream file;
	string imagename;
	int n = 0, correct = 0, j = 0, sum = 0;
	char filename[] = "E:\\general_test\\1.txt";
	string pathh = "E:\\general_test\\";
	file.open(filename, ios::in);
//	cout << filename << endl;
	while (!file.eof())
	{
		file >> imagename;
		string imgpath = pathh + imagename;
	//	cout << imgpath << endl;
		std::cout << "------------------------------------------------" << endl;
		cout << "ͼƬ����" << imagename << endl;
		cv::Mat image = cv::imread(imgpath);
		//		cv::imshow("image", image);
		//		cv::waitKey(0);

		std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_BASED_METHOD);

		float conf = 0;
		vector<float> con;
		vector<string> name;
		for (auto st : res) {
			if (st.confidence > 0.1) {
				//std::cout << st.getPlateName() << " " << st.confidence << std::endl;
				con.push_back(st.confidence);
				name.push_back(st.getPlateName());
				//conf += st.confidence;
			}
			else
				cout << "no string" << endl;
		}
		//	std::cout << conf << std::endl;
		int num = con.size();
		float max = 0;
		string platestr, chpr, ch;
		int diff = 0, dif = 0;
		for (int i = 0; i < num; i++) {

			if (con.at(i) > max)
			{
				max = con.at(i);
				platestr = name.at(i);
			}

		}
		//	cout << "max:"<<max << endl;
		cout << "string:" << platestr << endl;
		chpr = platestr.substr(0, 2);
		ch = imagename.substr(0, 2);
		diff = levenshtein_distance(imagename, platestr);
		dif = diff - 4;
		cout << "���:" << dif << endl;
		sum += dif;
		if (ch != chpr) n++;
		if (diff == 0)	correct++;
		j++;
	}
	float cha = 1 - float(n) / float(j);
	std::cout << "------------------------------------------------" << endl;
	cout << "����������" << j << endl;
	cout << "����ʶ��׼ȷ�ʣ�" << cha << endl;
	float chaccuracy = 1 - float(sum - n * 2) / float(j * 8);
	cout << "�ַ�ʶ��׼ȷ�ʣ�" << chaccuracy << endl;
	cv::waitKey(0);

}*/


void TEST_PIPELINE(){

	pr::PipelinePR prc("model/cascade.xml",
		"model/HorizonalFinemapping.prototxt", "model/HorizonalFinemapping.caffemodel",
		"model/Segmentation.prototxt", "model/Segmentation.caffemodel",
		"model/CharacterRecognization.prototxt", "model/CharacterRecognization.caffemodel",
		"model/SegmentationFree.prototxt", "model/SegmentationFree.caffemodel"
		);

	ofstream outtxt;
	outtxt.open("C:\\Users\\Michael\\Desktop\\result1.txt");
	ofstream missing;
	missing.open("C:\\Users\\Michael\\Desktop\\missing.txt");
	fs::path p("C:\\Users\\Michael\\Desktop\\plateRec\\plate_totally_done");
	int n = 0, correct = 0, j = 0, sum = 0;
	for (auto i = fs::directory_iterator(p); i != fs::directory_iterator(); i++)
	{
		string fileName1 = i->path().filename().string();
		cout << fileName1 << endl;
		i++;
		string fileName2 = i->path().filename().string();
		cout << fileName2 << endl;
		string pathName1 = "C:\\Users\\Michael\\Desktop\\plateRec\\plate_totally_done\\" + fileName1;
		string pathName2 = "C:\\Users\\Michael\\Desktop\\plateRec\\plate_totally_done\\" + fileName2;
		cv::Mat image = cv::imread(pathName1);
		string newName = fileName1.substr(0, 6);
		std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_FREE_METHOD, newName);
		if (res.size() == 0)
		{
			missing << fileName1 << endl;
			continue;
		}
		float conf = 0;
		vector<string> fileNames;
		vector<float> con;
		vector<string> name;
		for (auto st : res) {
			if (st.confidence > 0.1) {
				//std::cout << st.getPlateName() << " " << st.confidence << std::endl;
				con.push_back(st.confidence);
				name.push_back(st.getPlateName());
				fileNames.push_back(st.getFileName());
				cout << name.back() << endl;
				//conf += st.confidence;
			}
			else
				cout << "no string" << endl;
		}
		//	std::cout << conf << std::endl;
		int num = con.size();
		float max = 0;
		string fileName = "";
		string platestr, chpr, ch;
		int diff = 0, dif = 0;
		for (int i = 0; i < num; i++) {

			if (con.at(i) > max)
			{
				max = con.at(i);
				platestr = name.at(i);
				fileName = fileNames.at(i);
			}

		}
		//	cout << "max:"<<max << endl;
		outtxt << platestr << " ";
		chpr = platestr.substr(0, 2);
		char *fileName3 = (char*)pathName2.c_str();
		string imagename = util::readXML(fileName3);
		outtxt << imagename << " " << fileName << endl;
		ch = imagename.substr(0, 2);
		diff = levenshtein_distance(imagename, platestr);
		dif = diff - 4;
		cout << "���:" << dif << endl;
		sum += dif;
		if (ch != chpr) n++;
		if (diff == 0)	correct++;
		j++;
	}
	float cha = 1 - float(n) / float(j);
	std::cout << "------------------------------------------------" << endl;
	cout << "����������" << j << endl;
	cout << "����ʶ��׼ȷ�ʣ�" << cha << endl;
	float chaccuracy = 1 - float(sum - n * 2) / float(j * 8);
	cout << "�ַ�ʶ��׼ȷ�ʣ�" << chaccuracy << endl;
	outtxt.close();
	missing.close();
	cv::waitKey(0);
}
	

void TEST_ONE()
{
	pr::PipelinePR prc("model/cascade.xml",
		"model/HorizonalFinemapping.prototxt", "model/HorizonalFinemapping.caffemodel",
		"model/Segmentation.prototxt", "model/Segmentation.caffemodel",
		"model/CharacterRecognization.prototxt", "model/CharacterRecognization.caffemodel",
		"model/SegmentationFree.prototxt", "model/SegmentationFree.caffemodel"
	);
	cv::Mat image = cv::imread("C:\\Users\\Michael\\Desktop\\plateRec\\plate_totally_done\\000018-0-N1LH58.jpg");
	cv::imshow("1",image);
	cvWaitKey(10);
	string newName = "000018";
	std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(image, pr::SEGMENTATION_FREE_METHOD, newName);
	for (auto st : res) {
	if (st.confidence>0.75) {
	std::cout << st.getPlateName() << " " << st.confidence << std::endl;
	cv::Rect region = st.getPlateRect();

	cv::rectangle(image, cv::Point(region.x, region.y), cv::Point(region.x + region.width, region.y + region.height), cv::Scalar(255, 255, 0), 2);
	}
	}

	cv::imshow("image", image);
	cv::waitKey(0);
}

/*void TEST_CAM()
{

	cv::VideoCapture capture("test1.mp4");
	cv::Mat frame;

	pr::PipelinePR prc("model/cascade.xml",
		"model/HorizonalFinemapping.prototxt", "model/HorizonalFinemapping.caffemodel",
		"model/Segmentation.prototxt", "model/Segmentation.caffemodel",
		"model/CharacterRecognization.prototxt", "model/CharacterRecognization.caffemodel",
		"model/SegmentationFree.prototxt", "model/SegmentationFree.caffemodel"
		);





	while (1) {
		//��ȡ��һ֡
		if (!capture.read(frame)) {
			std::cout << "��ȡ��Ƶʧ��" << std::endl;
			exit(1);
		}
		//
		//        cv::transpose(frame,frame);
		//        cv::flip(frame,frame,2);

		//        cv::resize(frame,frame,cv::Size(frame.cols/2,frame.rows/2));



		std::vector<pr::PlateInfo> res = prc.RunPiplineAsImage(frame, pr::SEGMENTATION_FREE_METHOD);

		for (auto st : res) {
			if (st.confidence>0.75) {
				std::cout << st.getPlateName() << " " << st.confidence << std::endl;
				cv::Rect region = st.getPlateRect();

				cv::rectangle(frame, cv::Point(region.x, region.y), cv::Point(region.x + region.width, region.y + region.height), cv::Scalar(255, 255, 0), 2);
			}
		}
		cv::imshow("image", frame);
		cv::waitKey(1);



	}

}*/


int main()
{
	//TEST_ACC();
	//TEST_CAM();
	TEST_PIPELINE();
	//TEST_ONE();
	return 0;


}