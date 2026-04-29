#include <opencv2/cudafeatures2d.hpp>

int main()
{
    auto orb = cv::cuda::ORB::create();
    return orb.empty();
}
