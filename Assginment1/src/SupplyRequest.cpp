#include "SupplyRequest.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

// SupplyRequest implementation
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
    string statusString;
    switch (status)
    {
    case RequestStatus::PENDING:
        statusString = "PENDING";
        break;
    case RequestStatus::COLLECTING:
        statusString = "COLLECTING";
        break;
    case RequestStatus::ON_THE_WAY:
        statusString = "ON_THE_WAY";
        break;
    case RequestStatus::DONE:
        statusString = "DONE";
        break;
    }
    return "SupplyRequest{id=" + std::to_string(id) + ", beneficiaryId=" + std::to_string(beneficiaryId) + ", distance=" + std::to_string(distance) + ", status=" + statusString + ", inventoryManagerId=" + std::to_string(inventoryManagerId) + ", CourierId=" + std::to_string(CourierId) + "}";
}

// My functions
int SupplyRequest::getDistance() const { return distance; }