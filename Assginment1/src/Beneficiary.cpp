#include <Beneficiary.h>
#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;

// Beneficiary implementation
Beneficiary::Beneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : id(id), name(name), locationDistance(locationDistance), maxRequests(maxRequests),requestsId() {}

const string &Beneficiary::getName() const { return name; }
int Beneficiary::getId() const { return id; }
int Beneficiary::getBeneficiaryDistance() const { return locationDistance; }
int Beneficiary::getMaxRequests() const { return maxRequests; }
int Beneficiary::getNumRequests() const { return requestsId.size(); }
bool Beneficiary::canMakeRequest() const { return static_cast<int>(requestsId.size()) < maxRequests; }
const vector<int> &Beneficiary::getRequestsIds() const { return requestsId; }
int Beneficiary::addRequest(int RequestId)
{
    if (static_cast<int>(requestsId.size()) >= maxRequests)
    {
        return -1;
    }
    requestsId.push_back(RequestId);
    return RequestId;
}


// Added function to Beneficiary class
const std::string Beneficiary::toString() const{
    std::ostringstream oss;
    oss << "Beneficiary ID: " << getId() << "\n";
    oss << "Name: " << getName() << "\n"; //del
    oss << "Location Distance: " << getBeneficiaryDistance() << "\n"; //del
    oss << "Max Requests: " << getMaxRequests() << "\n"; //del
    oss << "Number of Requests: " << getNumRequests() << "\n"; //del
    for (auto &requestId : getRequestsIds()) // need to add the full info of each request
    {
        oss << "Request ID: " << requestId << "\n";
        oss << "Request Status: " << "\n"; // need to add the status of the request
    }
    oss << "Requests Left " << getMaxRequests() - getNumRequests() << "\n";
    return oss.str();

}

// HospitalBeneficiary implementation
HospitalBeneficiary::HospitalBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}
HospitalBeneficiary *HospitalBeneficiary::clone() const
{
    return new HospitalBeneficiary(*this);
}

// ClinicBeneficiary implementation
ClinicBeneficiary::ClinicBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}
ClinicBeneficiary *ClinicBeneficiary::clone() const
{
    return new ClinicBeneficiary(*this);
}