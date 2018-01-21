//===--- PIC.cpp - Implement PIC target feature support -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements PIC TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "PIC.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

namespace clang {
namespace targets {

/// Information about a specific microcontroller.
struct LLVM_LIBRARY_VISIBILITY MCUInfo {
  const char *Name;
  const char *DefineName;
};

// This list should be kept up-to-date with AVRDevices.td in LLVM.
static ArrayRef<MCUInfo> PICMcus = {
    {"at90s1200", "__AVR_AT90S1200__"},
};

} // namespace targets
} // namespace clang

bool PICTargetInfo::isValidCPUName(StringRef Name) const {
  bool IsMCU =
      std::find_if(PICMcus.begin(), PICMcus.end(), [&](const MCUInfo &Info) {
        return Info.Name == Name;
      }) != PICMcus.end();
  return IsMCU;
}

void PICTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  Builder.defineMacro("PIC");
  Builder.defineMacro("__PIC");
  Builder.defineMacro("__PIC__");

  if (!this->CPU.empty()) {
    auto It =
        std::find_if(PICMcus.begin(), PICMcus.end(), [&](const MCUInfo &Info) {
          return Info.Name == this->CPU;
        });

    if (It != PICMcus.end())
      Builder.defineMacro(It->DefineName);
  }
}
