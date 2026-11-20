#include "RedirectJumpHookBase.h"
#include "../../Utils/CPatch.h"

bool RedirectJumpHookBase::Inject()
{
    if (isInstalled_) {
        return false;
    }

    void *original = RedirectJumpOriginalFunctionAddress()->GetPointer();
    if (!original) {
        return false;
    }

    void *hook = RedirectJumpHookFunctionAddress()->GetPointer();
    if (!hook) {
        return false;
    }

    CPatch::SetBytes(reinterpret_cast<std::uintptr_t>(backupBytes_),
                     original,
                     redirectJumpInstructionSize);
    CPatch::RedirectJump(reinterpret_cast<std::uintptr_t>(original), hook);

    original_ = original;
    isInstalled_ = true;

    return true;
}

bool RedirectJumpHookBase::Uninstall()
{
    if (isInstalled_) {
        CPatch::SetBytes(reinterpret_cast<std::uintptr_t>(original_),
                         backupBytes_,
                         redirectJumpInstructionSize);
        original_ = nullptr;
        isInstalled_ = false;
        return true;
    } else {
        return false;
    }
}

RedirectJumpHookBase::~RedirectJumpHookBase()
{
    RedirectJumpHookBase::Uninstall();
}
