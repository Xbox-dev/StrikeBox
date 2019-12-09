// StrikeBox NV2A PGRAPH (2D/3D graphics engine) engine emulation
// (C) Ivan "StrikerX3" Oliveira
//
// Based on envytools:
// https://envytools.readthedocs.io/en/latest/index.html
//
// References to particular items in the documentation are denoted between
// brackets optionally followed by a quote from the documentation.
//
// PGRAPH is the main engine responsible for 2D/3D graphics command processing.
//
// PGRAPH engine registers occupy the range 0x400000..0x401FFF.
#pragma once

#include "engine.h"

namespace strikebox::nv2a {

// NV2A 2D/3D graphics engine (PGRAPH)
class PGRAPH : public NV2AEngine {
public:
    PGRAPH(NV2A& nv2a) : NV2AEngine("PGRAPH", 0x400000, 0x2000, nv2a) {
        Reset();
    }

    void SetEnabled(bool enabled);
    
    void Reset() override;
    uint32_t Read(const uint32_t addr) override;
    void Write(const uint32_t addr, const uint32_t value) override;

private:
    bool m_enabled = false;
};

}
