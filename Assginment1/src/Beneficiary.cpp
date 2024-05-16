#include <Beneficiary.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

// Beneficiary implementation
// ATT: I added the following constructor to the class BUT im not sure if needed, can there be more than 2 Beneficiary?
Beneficiary::Beneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : id(id), name(name), locationDistance(locationDistance), maxRequests(maxRequests) {}

const string &Beneficiary::getName() const { return name; }
int Beneficiary::getId() const { return id; }
int Beneficiary::getBeneficiaryDistance() const { return locationDistance; }
int Beneficiary::getMaxRequests() const { return maxRequests; }
int Beneficiary::getNumRequests() const { return requestsId.size(); }
bool Beneficiary::canMakeRequest() const { return requestsId.size() < maxRequests; }
const vector<int> &Beneficiary::getRequestsIds() const { return requestsId; }
int Beneficiary::addRequest(int RequestId)
{
    if (requestsId.size() >= maxRequests)
    {
        return -1;
    }
    requestsId.push_back(RequestId);
    return RequestId;
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
