// StrikeBox NV2A PRAMIN (RAMIN access) engine emulation
// (C) Ivan "StrikerX3" Oliveira
//
// Based on envytools and nouveau:
// https://envytools.readthedocs.io/en/latest/index.html
// https://github.com/torvalds/linux/tree/master/drivers/gpu/drm/nouveau
//
// References to particular items in the documentation are denoted between
// brackets optionally followed by a quote from the documentation.
#include "strikebox/hw/gpu/engines/pramin.h"

#include "strikebox/log.h"

namespace strikebox::nv2a {

void PRAMIN::Reset() {
    std::fill(m_mem, m_mem + m_length, 0);
}

uint32_t PRAMIN::Read(const uint32_t addr) {
    return *reinterpret_cast<uint32_t*>(&m_mem[addr]);
}

void PRAMIN::Write(const uint32_t addr, const uint32_t value) {
    *reinterpret_cast<uint32_t*>(&m_mem[addr]) = value;
}

}
