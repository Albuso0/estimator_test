#include <map>
#include <memory>

template <typename T>
class Support
{
public:
    Support();
    virtual ~Support();
	double estimate();
	void updateHist(std::map<T,int>* hist){ m_hist = hist; };

private:
	int k, n, L;
	std::unique_ptr< std::map<T, int> > m_hist;
	double estimate_poly();
	double estimate_plug();
};

