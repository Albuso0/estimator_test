#include <map>
#include <memory>

template <typename T>
class Support
{
public:
    Support(){};
    virtual ~Support(){};
	double estimate();
	void updateHist(std::shared_ptr< std::map<T, int> > vpHist){ mpHist = vpHist; };

private:
	int k, n, L;
	std::shared_ptr< std::map<T, int> > mpHist;
	double estimate_poly();
	double estimate_plug();
};

