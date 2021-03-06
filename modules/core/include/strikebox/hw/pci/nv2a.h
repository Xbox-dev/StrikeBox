#pragma once

#include <memory>

#include "pci.h"
#include "../basic/irq.h"
#include "../gpu/nv2a.h"

namespace strikebox {

class NV2ADevice : public PCIDevice {
public:
    NV2ADevice(uint8_t *pSystemRAM, uint32_t systemRAMSize, IRQHandler& irqHandler);
    virtual ~NV2ADevice();

    // PCI Device functions
    void Init();
    void Reset();
    
    void PCIIORead(int barIndex, uint32_t port, uint32_t *value, uint8_t size) override;
    void PCIIOWrite(int barIndex, uint32_t port, uint32_t value, uint8_t size) override;
    void PCIMMIORead(int barIndex, uint32_t addr, uint32_t *value, uint8_t size) override;
    void PCIMMIOWrite(int barIndex, uint32_t addr, uint32_t value, uint8_t size) override;

private:
    IRQHandler& m_irqHandler;

    // NV2A state
    std::unique_ptr<nv2a::NV2A> m_nv2a;
};

}
