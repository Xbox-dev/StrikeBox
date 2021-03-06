#pragma once

#include <assert.h>
#ifndef _WIN32
    #include <libgen.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "virt86/virt86.hpp"

#include "strikebox/log.h"
#include "strikebox/mem.h"
#include "strikebox/util.h"
#include "strikebox/thread.h"
#include "strikebox/settings.h"
#include "strikebox/status.h"

#include "strikebox/hw/basic/irq.h"
#include "strikebox/hw/basic/gsi.h"
#include "strikebox/hw/basic/i8254.h"
#include "strikebox/hw/basic/i8259.h"
#include "strikebox/hw/basic/superio.h"
#include "strikebox/hw/basic/cmos.h"

#include "strikebox/hw/ata/ata.h"

#include "strikebox/hw/bus/smbus.h"
#include "strikebox/hw/bus/pcibus.h"

#include "strikebox/hw/sm/smc.h"
#include "strikebox/hw/sm/eeprom.h"
#include "strikebox/hw/sm/tvenc.h"
#include "strikebox/hw/sm/adm1032.h"

#include "strikebox/hw/pci/hostbridge.h"
#include "strikebox/hw/pci/mcpx_ram.h"
#include "strikebox/hw/pci/lpc.h"
#include "strikebox/hw/pci/agpbridge.h"
#include "strikebox/hw/pci/pcibridge.h"
#include "strikebox/hw/pci/usb_pci.h"
#include "strikebox/hw/pci/nvnet.h"
#include "strikebox/hw/pci/nvapu.h"
#include "strikebox/hw/pci/ac97.h"
#include "strikebox/hw/pci/bmide.h"
#include "strikebox/hw/pci/nv2a.h"

namespace strikebox {

/*!
 * Xbox kernel version data structure exported by Microsoft kernels.
 */
struct XboxKernelVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t build;
    uint16_t rev;
};

/*!
 * Top-level Xbox machine class
 *
 * This class is the top-level class, will perform initialization and high-level
 * management of the overall emulation flow.
 */
class Xbox {
public:
    Xbox(virt86::Platform& platform);
    virtual ~Xbox();

    StrikeBoxSettings& GetSettings() { return m_settings; }
    void CopySettings(StrikeBoxSettings *settings);

    EmulatorStatus Run();
    void Stop();

protected:
    // ----- Initialization and cleanup ---------------------------------------
    EmulatorStatus Initialize();
    EmulatorStatus InitFixupSettings();
    EmulatorStatus InitVM();
    EmulatorStatus InitMemory();
    EmulatorStatus InitRAM();
    EmulatorStatus InitROM();
    EmulatorStatus InitHardware();
    EmulatorStatus InitDebugger();

    void Cleanup();

    // ----- Thread functions -------------------------------------------------
    static uint32_t EmuCpuThreadFunc(void *data);
    int RunCpu();

    // ----- I/O callbacks ----------------------------------------------------
    static uint32_t IOReadCallback(void *context, uint16_t port, size_t size);
    static void IOWriteCallback(void *context, uint16_t port, size_t size, uint32_t value);
    static uint64_t MMIOReadCallback(void *context, uint64_t address, size_t size);
    static void MMIOWriteCallback(void *context, uint64_t address, size_t size, uint64_t value);

    uint32_t IORead(uint16_t port, size_t size);
    void IOWrite(uint16_t port, size_t size, uint32_t value);
    uint64_t MMIORead(uint64_t address, size_t size);
    void MMIOWrite(uint64_t address, size_t size, uint64_t value);

    // ----- Hardware ---------------------------------------------------------
    virt86::Platform& m_virt86Platform;
    std::optional<std::reference_wrapper<virt86::VirtualMachine>> m_vm;

    uint32_t          m_ramSize = 0;
    uint8_t          *m_ram = nullptr;
    uint8_t          *m_rom = nullptr;
    uint8_t          *m_bios = nullptr;
    uint32_t          m_biosSize = 0;
    uint8_t          *m_mcpxROM = nullptr;
    IOMapper          m_ioMapper;

    GSI              *m_GSI = nullptr;
    IRQ              *m_IRQs = nullptr;
    IRQ              *m_acpiIRQs = nullptr;
    IRQ              *m_i8259IRQs = nullptr;

    i8254            *m_i8254 = nullptr;
    i8259            *m_i8259 = nullptr;
    CMOS             *m_CMOS = nullptr;
    hw::ata::ATA     *m_ATA = nullptr;
    hw::ata::IATADeviceDriver *m_ataDrivers[2][2] = { { nullptr } };
    CharDriver       *m_CharDrivers[SUPERIO_SERIAL_PORT_COUNT] = { nullptr };
    SuperIO          *m_SuperIO = nullptr;

    SMBus            *m_SMBus = nullptr;
    SMCDevice        *m_SMC = nullptr;
    EEPROMDevice     *m_EEPROM = nullptr;
    TVEncoderDevice  *m_TVEncoder = nullptr;
    ADM1032Device    *m_ADM1032 = nullptr;

    PCIBus           *m_PCIBus = nullptr;
    HostBridgeDevice *m_HostBridge = nullptr;
    MCPXRAMDevice    *m_MCPXRAM = nullptr;
    LPCDevice        *m_LPC = nullptr;
    USBPCIDevice     *m_USB1 = nullptr;
    USBPCIDevice     *m_USB2 = nullptr;
    NVNetDevice      *m_NVNet = nullptr;
    NVAPUDevice      *m_NVAPU = nullptr;
    AC97Device       *m_AC97 = nullptr;
    PCIBridgeDevice  *m_PCIBridge = nullptr;
    hw::bmide::BMIDEDevice *m_BMIDE = nullptr;
    AGPBridgeDevice  *m_AGPBridge = nullptr;
    NV2ADevice       *m_NV2A = nullptr;

    // ----- Configuration ----------------------------------------------------
    StrikeBoxSettings     m_settings;

    // ----- State ------------------------------------------------------------
    bool     m_should_run;

    uint8_t  m_lastSMCErrorCode = 0;
    uint32_t m_lastBugCheckCode = 0x00000000;
    
    XboxKernelVersion m_kernelVersion = { 0 };

    bool     m_kernelDataFound = false;
    uint32_t m_kExp_KiBugCheckData = 0x00000000;
    uint32_t m_kExp_XboxKrnlVersion = 0x00000000;

    bool LocateKernelData();
};

}
