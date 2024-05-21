#include <iostream>
#include "SupplyRequest.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// SupplyRequest implementation
SupplyRequest::SupplyRequest() : id(-1), beneficiaryId(-1), distance(-1), status(RequestStatus::PENDING), inventoryManagerId(NO_VOLUNTEER), CourierId(NO_VOLUNTEER) {}

SupplyRequest::SupplyRequest(int id, int beneficiaryId, int distance)
    : id(id), beneficiaryId(beneficiaryId), distance(distance), status(RequestStatus::PENDING), inventoryManagerId(NO_VOLUNTEER), CourierId(NO_VOLUNTEER) {}
int SupplyRequest::getId() const { return id; }
int SupplyRequest::getBeneficiaryId() const { return beneficiaryId; }
void SupplyRequest::setStatus(RequestStatus status)
{
    if (status == RequestStatus::PENDING || status == RequestStatus::COLLECTING || status == RequestStatus::ON_THE_WAY || status == RequestStatus::DONE)
    {
        this->status = status;
    }
    else
    {
        throw "Invalid status"; // Im not sure about what to do when its not working
    }
}
void SupplyRequest::setInventoryManagerId(int inventoryManagerId)
{
    this->inventoryManagerId = inventoryManagerId;
}
void SupplyRequest::setCourierId(int CourierId)
{
    this->CourierId = CourierId;
}
int SupplyRequest::getInventoryManagerId() const { return inventoryManagerId; }
int SupplyRequest::getCourierId() const { return CourierId; }
RequestStatus SupplyRequest::getStatus() const { return status; }
const std::string SupplyRequest::toString() const
{
    std::ostringstream oss;
    oss << "Request ID: " << getId() << "\n";
    oss << "Status: ";

    switch (getStatus())
    {
    case RequestStatus::PENDING:
        oss << "Pending";
        break;
    case RequestStatus::COLLECTING:
        oss << "Collecting";
        break;
    case RequestStatus::ON_THE_WAY:
        oss << "On the Way";
        break;
    case RequestStatus::DONE:
        oss << "Done";
        break;
    }
    oss << "\n";

    oss << "Beneficiary ID: " << getBeneficiaryId() << "\n";

    if (getInventoryManagerId() != NO_VOLUNTEER)
    {
        oss << "Inventory Manager: " << getInventoryManagerId() << "\n";
    }
    else
    {
        oss << "Inventory Manager: None" << "\n";
    }

    if (getCourierId() != NO_VOLUNTEER)
    {
        oss << "Courier: " << getCourierId() << "\n";
    }
    else
    {
        oss << "Courier: None" << "\n";
    }

    return oss.str();
}

// My functions
int SupplyRequest::getDistance() const { return distance; }
