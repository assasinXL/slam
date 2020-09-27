#include <exception>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

Mat img1, img2;
void ORB_demo(int, void *);
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "usage: show_orb_features <pic1_path> <pic2_path>" << endl;
    return 0;
  }
  img1 = imread(argv[1]);
  img2 = imread(argv[2]);
  if (!img1.data || !img2.data)
    throw exception("picture not found!");
  namedWindow("ORB_demo", CV_WINDOW_AUTOSIZE);

  ORB_demo(0, 0);

  imshow("input image of box", img1);
  imshow("input image of box_in_scene", img2);

  waitKey(0);
  return 0;
}

/*---------------检测与匹配--------------*/
void ORB_demo(int, void *) {
  int Hession = 400;
  double t1 = static_cast<double>(getTickCount());
  //特征点提取
  Ptr<ORB> detector = ORB::create(400);
  vector<KeyPoint> keypoints_obj;
  vector<KeyPoint> keypoints_scene;
  //定义描述子
  Mat descriptor_obj, descriptor_scene;
  //检测并计算成描述子
  detector->detectAndCompute(img1, Mat(), keypoints_obj, descriptor_obj);
  detector->detectAndCompute(img2, Mat(), keypoints_scene, descriptor_scene);

  double t2 = static_cast<double>(getTickCount());
  double t = (t2 - t1) * 1000 / getTickFrequency();
  //特征匹配
  FlannBasedMatcher fbmatcher(new flann::LshIndexParams(20, 10, 2));
  vector<DMatch> matches;
  //将找到的描述子进行匹配并存入matches中
  fbmatcher.match(descriptor_obj, descriptor_scene, matches);

  double minDist = 1000;
  double maxDist = 0;
  //找出最优描述子
  vector<DMatch> goodmatches;
  for (int i = 0; i < descriptor_obj.rows; i++) {
    double dist = matches[i].distance;
    if (dist < minDist) {
      minDist = dist;
    }
    if (dist > maxDist) {
      maxDist = dist;
    }
  }
  for (int i = 0; i < descriptor_obj.rows; i++) {
    double dist = matches[i].distance;
    if (dist < max(2 * minDist, 0.02)) {
      goodmatches.push_back(matches[i]);
    }
  }
  Mat orbImg;

  drawMatches(img1, keypoints_obj, img2, keypoints_scene, goodmatches, orbImg,
              Scalar::all(-1), Scalar::all(-1), vector<char>(),
              DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

  //----------目标物体用矩形标识出来------------
  vector<Point2f> obj;
  vector<Point2f> scene;
  for (size_t i = 0; i < goodmatches.size(); i++) {
    obj.push_back(keypoints_obj[goodmatches[i].queryIdx].pt);
    scene.push_back(keypoints_scene[goodmatches[i].trainIdx].pt);
  }
  vector<Point2f> obj_corner(4);
  vector<Point2f> scene_corner(4);
  //生成透视矩阵
  Mat H = findHomography(obj, scene, RANSAC);

  obj_corner[0] = Point(0, 0);
  obj_corner[1] = Point(img1.cols, 0);
  obj_corner[2] = Point(img1.cols, img1.rows);
  obj_corner[3] = Point(0, img1.rows);
  //透视变换
  perspectiveTransform(obj_corner, scene_corner, H);
  Mat resultImg = orbImg.clone();

  for (int i = 0; i < 4; i++) {
    line(resultImg, scene_corner[i] + Point2f(static_cast<float>(img1.cols), 0),
         scene_corner[(i + 1) % 4] + Point2f(static_cast<float>(img1.cols), 0),
         Scalar(0, 0, 255), 2, 8, 0);
  }
  imshow("result image", resultImg);

  cout << "ORB execute time: " << t << "ms" << endl;
  cout << "Min distance: " << minDist << endl;
  cout << "Max distance: " << maxDist << endl;
  imshow("ORB_demo", orbImg);
}