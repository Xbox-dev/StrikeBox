// StrikeBox NV2A PVIDEO (Video overlay) engine emulation
// (C) Ivan "StrikerX3" Oliveira
//
// Based on envytools and nouveau:
// https://envytools.readthedocs.io/en/latest/index.html
// https://github.com/torvalds/linux/tree/master/drivers/gpu/drm/nouveau
//
// References to particular items in the documentation are denoted between
// brackets optionally followed by a quote from the documentation.
//
// PVIDEO engine registers occupy the range 0x008000..0x008FFF.
#pragma once

#include "../engine.h"

namespace strikebox::nv2a {

// PVIDEO registers
// [https://envytools.readthedocs.io/en/latest/hw/display/nv3/pvideo.html#mmio-registers]
const uint32_t Reg_PVIDEO_INTR = 0x100;        // [RW] Interrupt status
const uint32_t Reg_PVIDEO_INTR_ENABLE = 0x140; // [RW] Interrupt enable

// ----------------------------------------------------------------------------

// NV2A video overlay engine (PVIDEO)
class PVIDEO : public NV2AEngine {
public:
    PVIDEO(NV2A& nv2a) : NV2AEngine("PVIDEO", 0x008000, 0x1000, nv2a) {}

    void SetEnabled(bool enabled);

    void Reset() override;
    uint32_t Read(const uint32_t addr) override;
    void Write(const uint32_t addr, const uint32_t value) override;

    bool GetInterruptState() { return m_interruptLevels & m_enabledInterrupts; }

private:
    bool m_enabled = false;

    uint32_t m_interruptLevels;
    uint32_t m_enabledInterrupts;
};

}
