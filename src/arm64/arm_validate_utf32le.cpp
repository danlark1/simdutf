
const char32_t* arm_validate_utf32le(const char32_t* input, size_t size) {
    const char32_t* end = input + size;

    const uint32x4_t standardmax = vmovq_n_u32(0x10ffff);
    const uint32x4_t offset = vmovq_n_u32(0xffff2000);
    const uint32x4_t standardoffsetmax = vmovq_n_u32(0xfffff7ff);
    uint32x4_t currentmax = vmovq_n_u32(0x0);
    uint32x4_t currentoffsetmax = vmovq_n_u32(0x0);

    while (input + 4 < end) {
        const uint32x4_t in = vld1q_u32(reinterpret_cast<const uint32_t*>(input));
        currentmax = vmaxq_u32(in,currentmax);
        currentoffsetmax = vmaxq_u32(vaddq_u32(in, offset), currentoffsetmax);
        input += 4;
    }

    uint32x4_t is_zero = veorq_u32(vmaxq_u32(currentmax, standardmax), standardmax);
    if(vmaxvq_u32(is_zero) != 0) {
        return nullptr;
    }

    is_zero = veorq_u32(vmaxq_u32(currentoffsetmax, standardoffsetmax), standardoffsetmax);
    if(vmaxvq_u32(is_zero) != 0) {
        return nullptr;
    }

    return input;
}
