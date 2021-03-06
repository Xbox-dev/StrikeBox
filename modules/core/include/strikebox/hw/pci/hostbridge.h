#pragma once

#include <cstdint>

#include "../defs.h"
#include "pci.h"

namespace strikebox {

class HostBridgeDevice : public PCIDevice {
public:
    // constructor
    HostBridgeDevice();
    virtual ~HostBridgeDevice();

    // PCI Device functions
    void Init();
    void Reset();

    void PCIMMIORead(int barIndex, uint32_t addr, uint32_t *value, uint8_t size) override;
    void PCIMMIOWrite(int barIndex, uint32_t addr, uint32_t value, uint8_t size) override;
};

}
