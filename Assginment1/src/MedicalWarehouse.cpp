#include <string>
#include <vector>

#include "SupplyRequest.h"
#include "Beneficiary.h"
#include <MedicalWarehouse.h>

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
    : isOpen(true), actionsLog(), volunteers(), pendingRequests(), inProcessRequests(), completedRequests(), Beneficiaries(), beneficiaryCounter(0), volunteerCounter(0) {}

//ATT: NEED TO IMPLEMENT THE START FUNCTION
void MedicalWareHouse::start() {}

void MedicalWareHouse::addRequest(SupplyRequest *request)
{
    // ATT: Do I need to make checks for the request?
    pendingRequests.push_back(request);
}
void MedicalWareHouse::addAction(CoreAction *action)
{
    actionsLog.push_back(action);
}
Beneficiary &MedicalWareHouse::getBeneficiary(int beneficiaryId) const
{
    for (auto &beneficiary : Beneficiaries)
    {
        if (beneficiary->getId() == beneficiaryId)
        {
            return *beneficiary;
        }
    }
    throw "Beneficiary not found";
}
#include "Volunteer.h"

Volunteer &MedicalWareHouse::getVolunteer(int volunteerId) const
{
    for (auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw "Volunteer not found";
}
SupplyRequest &MedicalWareHouse::getRequest(int requestId) const
{
    for (auto &request : pendingRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    for (auto &request : inProcessRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    for (auto &request : completedRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    throw "Request not found";
}
const vector<CoreAction *> &MedicalWareHouse::getActions() const { return actionsLog; }
void MedicalWareHouse::close() { isOpen = false; }
void MedicalWareHouse::open() { isOpen = true; }
