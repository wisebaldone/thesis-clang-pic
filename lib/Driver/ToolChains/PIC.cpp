//===--- PIC.cpp - PIC ToolChain Implementations ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "PIC.h"
#include "CommonArgs.h"
#include "InputInfo.h"
#include "clang/Driver/Compilation.h"
#include "llvm/Option/ArgList.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

/// PIC Toolchain
PICToolChain::PICToolChain(const Driver &D, const llvm::Triple &Triple,
                           const ArgList &Args)
  : Generic_ELF(D, Triple, Args) { }
Tool *PICToolChain::buildLinker() const {
  return new tools::PIC::Linker(*this);
}

void PIC::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                               const InputInfo &Output,
                               const InputInfoList &Inputs,
                               const ArgList &Args,
                               const char *LinkingOutput) const {

  std::string Linker = getToolChain().GetProgramPath(getShortName());
  ArgStringList CmdArgs;
  CmdArgs.push_back("-flavor");
  CmdArgs.push_back("gnu");
  CmdArgs.push_back("-target");
  CmdArgs.push_back(getToolChain().getTripleString().c_str());
  AddLinkerInputs(getToolChain(), Inputs, Args, CmdArgs, JA);
  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());
  C.addCommand(llvm::make_unique<Command>(JA, *this, Args.MakeArgString(Linker),
                                          CmdArgs, Inputs));
}
// PIC tools end.
