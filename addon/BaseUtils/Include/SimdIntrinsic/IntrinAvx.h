namespace SimdOpt
{
SIMD_SCOPE_BEGIN(INTRIN_MODE)
SIMD_DEFINE_SCOPVARS(INTRIN_MODE)

inline __m256i _v256_combine(const __m128i& lo, const __m128i& hi)
{ return _mm256_inserti128_si256(_mm256_castsi128_si256(lo), hi, 1); }

inline __m256 _v256_combine(const __m128& lo, const __m128& hi)
{ return _mm256_insertf128_ps(_mm256_castps128_ps256(lo), hi, 1); }

inline __m256d _v256_combine(const __m128d& lo, const __m128d& hi)
{ return _mm256_insertf128_pd(_mm256_castpd128_pd256(lo), hi, 1); }

inline int32_t _v_cvtsi256_si32(const __m256i& a)
{ return _mm_cvtsi128_si32(_mm256_castsi256_si128(a)); }

inline __m256i _v256_shuffle_odd_64(const __m256i& v)
{ return _mm256_permute4x64_epi64(v, _MM_SHUFFLE(3, 1, 2, 0)); }

inline __m256d _v256_shuffle_odd_64(const __m256d& v)
{ return _mm256_permute4x64_pd(v, _MM_SHUFFLE(3, 1, 2, 0)); }

template<int32_t imm>
inline __m256i _v256_permute2x128(const __m256i& a, const __m256i& b)
{ return _mm256_permute2x128_si256(a, b, imm); }

template<int32_t imm>
inline __m256 _v256_permute2x128(const __m256& a, const __m256& b)
{ return _mm256_permute2f128_ps(a, b, imm); }

template<int32_t imm>
inline __m256d _v256_permute2x128(const __m256d& a, const __m256d& b)
{ return _mm256_permute2f128_pd(a, b, imm); }

template<int32_t imm, typename _Tpvec>
inline _Tpvec v256_permute2x128(const _Tpvec& a, const _Tpvec& b)
{ return _Tpvec(_v256_permute2x128<imm>(a.val, b.val)); }

template<int32_t imm>
inline __m256i _v256_permute4x64(const __m256i& a)
{ return _mm256_permute4x64_epi64(a, imm); }

template<int32_t imm>
inline __m256d _v256_permute4x64(const __m256d& a)
{ return _mm256_permute4x64_pd(a, imm); }

template<int32_t imm, typename _Tpvec>
inline _Tpvec v256_permute4x64(const _Tpvec& a)
{ return _Tpvec(_v256_permute4x64<imm>(a.val)); }

inline __m128i _v256_extract_high(const __m256i& v)
{ return _mm256_extracti128_si256(v, 1); }

inline __m128  _v256_extract_high(const __m256& v)
{ return _mm256_extractf128_ps(v, 1); }

inline __m128d _v256_extract_high(const __m256d& v)
{ return _mm256_extractf128_pd(v, 1); }

inline __m128i _v256_extract_low(const __m256i& v)
{ return _mm256_castsi256_si128(v); }

inline __m128  _v256_extract_low(const __m256& v)
{ return _mm256_castps256_ps128(v); }

inline __m128d _v256_extract_low(const __m256d& v)
{ return _mm256_castpd256_pd128(v); }

inline __m256i _v256_packs_epu32(const __m256i& a, const __m256i& b)
{
    const __m256i m = _mm256_set1_epi32(65535);
    __m256i am = _mm256_min_epu32(a, m);
    __m256i bm = _mm256_min_epu32(b, m);
    return _mm256_packus_epi32(am, bm);
}

template<int32_t i>
inline int32_t _v256_extract_epi8(const __m256i& a)
{
#if defined(USE_AVX2)
    return _mm256_extract_epi8(a, i);
#else
    __m128i b = _mm256_extractf128_si256(a, ((i) >> 4));
    return _mm_extract_epi8(b, i & 15);  // SSE4.1
#endif
}

template<int32_t i>
inline int32_t _v256_extract_epi16(const __m256i& a)
{
#if defined(USE_AVX2)
    return _mm256_extract_epi16(a, i);
#else
    __m128i b = _mm256_extractf128_si256(a, ((i) >> 3));
    return _mm_extract_epi16(b, i & 7);  // SSE2
#endif
}

template<int32_t i>
inline int32_t _v256_extract_epi32(const __m256i& a)
{
#if defined(USE_AVX2)
    return _mm256_extract_epi32(a, i);
#else
    __m128i b = _mm256_extractf128_si256(a, ((i) >> 2));
    return _mm_extract_epi32(b, i & 3);  // SSE4.1
#endif
}

template<int32_t i>
inline int64_t _v256_extract_epi64(const __m256i& a)
{
#if defined(USE_AVX2)
    return _mm256_extract_epi64(a, i);
#else
    __m128i b = _mm256_extractf128_si256(a, ((i) >> 1));
    return _mm_extract_epi64(b, i & 1);  // SSE4.1
#endif
}

///////// Types ////////////

struct v_uint8x32
{
    typedef uint8_t lane_type;
    enum { nlanes = 32 };
    __m256i val;

    explicit v_uint8x32(__m256i v) : val(v) {}
    v_uint8x32(uint8_t v0,  uint8_t v1,  uint8_t v2,  uint8_t v3,
               uint8_t v4,  uint8_t v5,  uint8_t v6,  uint8_t v7,
               uint8_t v8,  uint8_t v9,  uint8_t v10, uint8_t v11,
               uint8_t v12, uint8_t v13, uint8_t v14, uint8_t v15,
               uint8_t v16, uint8_t v17, uint8_t v18, uint8_t v19,
               uint8_t v20, uint8_t v21, uint8_t v22, uint8_t v23,
               uint8_t v24, uint8_t v25, uint8_t v26, uint8_t v27,
               uint8_t v28, uint8_t v29, uint8_t v30, uint8_t v31)
    {
        val = _mm256_setr_epi8((int8_t)v0, (int8_t)v1, (int8_t)v2, (int8_t)v3,
            (int8_t)v4,  (int8_t)v5,  (int8_t)v6 , (int8_t)v7,  (int8_t)v8,  (int8_t)v9,
            (int8_t)v10, (int8_t)v11, (int8_t)v12, (int8_t)v13, (int8_t)v14, (int8_t)v15,
            (int8_t)v16, (int8_t)v17, (int8_t)v18, (int8_t)v19, (int8_t)v20, (int8_t)v21,
            (int8_t)v22, (int8_t)v23, (int8_t)v24, (int8_t)v25, (int8_t)v26, (int8_t)v27,
            (int8_t)v28, (int8_t)v29, (int8_t)v30, (int8_t)v31);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_uint8x32() {}

    uint8_t get0() const { return (uint8_t)_v_cvtsi256_si32(val); }
};

struct v_int8x32
{
    typedef int8_t lane_type;
    enum { nlanes = 32 };
    __m256i val;

    explicit v_int8x32(__m256i v) : val(v) {}
    v_int8x32(int8_t v0,  int8_t v1,  int8_t v2,  int8_t v3,
              int8_t v4,  int8_t v5,  int8_t v6,  int8_t v7,
              int8_t v8,  int8_t v9,  int8_t v10, int8_t v11,
              int8_t v12, int8_t v13, int8_t v14, int8_t v15,
              int8_t v16, int8_t v17, int8_t v18, int8_t v19,
              int8_t v20, int8_t v21, int8_t v22, int8_t v23,
              int8_t v24, int8_t v25, int8_t v26, int8_t v27,
              int8_t v28, int8_t v29, int8_t v30, int8_t v31)
    {
        val = _mm256_setr_epi8(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
            v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20,
            v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_int8x32() {}

    int8_t get0() const { return (int8_t)_v_cvtsi256_si32(val); }
};

struct v_uint16x16
{
    typedef uint16_t lane_type;
    enum { nlanes = 16 };
    __m256i val;

    explicit v_uint16x16(__m256i v) : val(v) {}
    v_uint16x16(uint16_t v0,  uint16_t v1,  uint16_t v2,  uint16_t v3,
                uint16_t v4,  uint16_t v5,  uint16_t v6,  uint16_t v7,
                uint16_t v8,  uint16_t v9,  uint16_t v10, uint16_t v11,
                uint16_t v12, uint16_t v13, uint16_t v14, uint16_t v15)
    {
        val = _mm256_setr_epi16((int16_t)v0, (int16_t)v1, (int16_t)v2, (int16_t)v3,
            (int16_t)v4,  (int16_t)v5,  (int16_t)v6,  (int16_t)v7,  (int16_t)v8,  (int16_t)v9,
            (int16_t)v10, (int16_t)v11, (int16_t)v12, (int16_t)v13, (int16_t)v14, (int16_t)v15);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_uint16x16() {}

    uint16_t get0() const { return (uint16_t)_v_cvtsi256_si32(val); }
};

struct v_int16x16
{
    typedef int16_t lane_type;
    enum { nlanes = 16 };
    __m256i val;

    explicit v_int16x16(__m256i v) : val(v) {}
    v_int16x16(int16_t v0,  int16_t v1,  int16_t v2,  int16_t v3,
               int16_t v4,  int16_t v5,  int16_t v6,  int16_t v7,
               int16_t v8,  int16_t v9,  int16_t v10, int16_t v11,
               int16_t v12, int16_t v13, int16_t v14, int16_t v15)
    {
        val = _mm256_setr_epi16(v0, v1, v2, v3, v4, v5, v6, v7,
            v8, v9, v10, v11, v12, v13, v14, v15);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_int16x16() {}

    int16_t get0() const { return (int16_t)_v_cvtsi256_si32(val); }
};

struct v_uint32x8
{
    typedef uint32_t lane_type;
    enum { nlanes = 8 };
    __m256i val;

    explicit v_uint32x8(__m256i v) : val(v) {}
    v_uint32x8(uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3,
               uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7)
    {
        val = _mm256_setr_epi32((uint32_t)v0, (uint32_t)v1, (uint32_t)v2,
            (uint32_t)v3, (uint32_t)v4, (uint32_t)v5, (uint32_t)v6, (uint32_t)v7);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_uint32x8() {}

    uint32_t get0() const { return (uint32_t)_v_cvtsi256_si32(val); }
};

struct v_int32x8
{
    typedef int32_t lane_type;
    enum { nlanes = 8 };
    __m256i val;

    explicit v_int32x8(__m256i v) : val(v) {}
    v_int32x8(int32_t v0, int32_t v1, int32_t v2, int32_t v3,
              int32_t v4, int32_t v5, int32_t v6, int32_t v7)
    {
        val = _mm256_setr_epi32(v0, v1, v2, v3, v4, v5, v6, v7);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_int32x8() {}

    int32_t get0() const { return _v_cvtsi256_si32(val); }
};

struct v_float32x8
{
    typedef float lane_type;
    enum { nlanes = 8 };
    __m256 val;

    explicit v_float32x8(__m256 v) : val(v) {}
    v_float32x8(float v0, float v1, float v2, float v3,
                float v4, float v5, float v6, float v7)
    {
        val = _mm256_setr_ps(v0, v1, v2, v3, v4, v5, v6, v7);
    }
    /* coverity[uninit_ctor]: suppress warning */
    v_float32x8() {}

    float get0() const { return _mm_cvtss_f32(_mm256_castps256_ps128(val)); }
};

struct v_uint64x4
{
    typedef uint64_t lane_type;
    enum { nlanes = 4 };
    __m256i val;

    explicit v_uint64x4(__m256i v) : val(v) {}
    v_uint64x4(uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3)
    { val = _mm256_setr_epi64x((int64_t)v0, (int64_t)v1, (int64_t)v2, (int64_t)v3); }
    /* coverity[uninit_ctor]: suppress warning */
    v_uint64x4() {}

    uint64_t get0() const
    {
    #if defined __x86_64__ || defined _M_X64
        return (uint64_t)_mm_cvtsi128_si64(_mm256_castsi256_si128(val));
    #else
        int32_t a = _mm_cvtsi128_si32(_mm256_castsi256_si128(val));
        int32_t b = _mm_cvtsi128_si32(_mm256_castsi256_si128(_mm256_srli_epi64(val, 32)));
        return (uint32_t)a | ((uint64_t)(uint32_t)b << 32);
    #endif
    }
};

struct v_int64x4
{
    typedef int64_t lane_type;
    enum { nlanes = 4 };
    __m256i val;

    explicit v_int64x4(__m256i v) : val(v) {}
    v_int64x4(int64_t v0, int64_t v1, int64_t v2, int64_t v3)
    { val = _mm256_setr_epi64x(v0, v1, v2, v3); }
    /* coverity[uninit_ctor]: suppress warning */
    v_int64x4() {}

    int64_t get0() const
    {
    #if defined __x86_64__ || defined _M_X64
        return (int64_t)_mm_cvtsi128_si64(_mm256_castsi256_si128(val));
    #else
        int32_t a = _mm_cvtsi128_si32(_mm256_castsi256_si128(val));
        int32_t b = _mm_cvtsi128_si32(_mm256_castsi256_si128(_mm256_srli_epi64(val, 32)));
        return (int64_t)((uint32_t)a | ((uint64_t)(uint32_t)b << 32));
    #endif
    }
};

struct v_float64x4
{
    typedef double lane_type;
    enum { nlanes = 4 };
    __m256d val;

    explicit v_float64x4(__m256d v) : val(v) {}
    v_float64x4(double v0, double v1, double v2, double v3)
    { val = _mm256_setr_pd(v0, v1, v2, v3); }
    /* coverity[uninit_ctor]: suppress warning */
    v_float64x4() {}

    double get0() const { return _mm_cvtsd_f64(_mm256_castpd256_pd128(val)); }
};

//////////////// Load and store operations ///////////////
#define SIMD_LOADSTORE_INT(_Tpvec, _Tp)                                 \
    inline _Tpvec v256_load(const _Tp* ptr)                             \
    { return _Tpvec(_mm256_loadu_si256((const __m256i*)ptr)); }         \
    inline _Tpvec v256_load_aligned(const _Tp* ptr)                     \
    { return _Tpvec(_mm256_load_si256((const __m256i*)ptr)); }          \
    inline _Tpvec v256_load_low(const _Tp* ptr)                         \
    {                                                                   \
        __m128i v128 = _mm_loadu_si128((const __m128i*)ptr);            \
        return _Tpvec(_mm256_castsi128_si256(v128));                    \
    }                                                                   \
    inline _Tpvec v256_load_halves(const _Tp* ptr0, const _Tp* ptr1)    \
    {                                                                   \
        __m128i vlo = _mm_loadu_si128((const __m128i*)ptr0);            \
        __m128i vhi = _mm_loadu_si128((const __m128i*)ptr1);            \
        return _Tpvec(_v256_combine(vlo, vhi));                         \
    }                                                                   \
    inline void v_store(_Tp* ptr, const _Tpvec& a)                      \
    { _mm256_storeu_si256((__m256i*)ptr, a.val); }                      \
    inline void v_store_aligned(_Tp* ptr, const _Tpvec& a)              \
    { _mm256_store_si256((__m256i*)ptr, a.val); }                       \
    inline void v_store_aligned_nocache(_Tp* ptr, const _Tpvec& a)      \
    { _mm256_stream_si256((__m256i*)ptr, a.val); }                      \
    inline void v_store(_Tp* ptr, const _Tpvec& a, StoreMode mode)      \
    {                                                                   \
        if (mode == StoreMode::ADDR_UNALIGNED)                               \
            _mm256_storeu_si256((__m256i*)ptr, a.val);                  \
        else if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)                    \
            _mm256_stream_si256((__m256i*)ptr, a.val);                  \
        else                                                            \
            _mm256_store_si256((__m256i*)ptr, a.val);                   \
    }                                                                   \
    inline void v_store_low(_Tp* ptr, const _Tpvec& a)                  \
    { _mm_storeu_si128((__m128i*)ptr, _v256_extract_low(a.val)); }      \
    inline void v_store_high(_Tp* ptr, const _Tpvec& a)                 \
    { _mm_storeu_si128((__m128i*)ptr, _v256_extract_high(a.val)); }

SIMD_LOADSTORE_INT(v_uint8x32,  uint8_t)
SIMD_LOADSTORE_INT(v_int8x32,   int8_t)
SIMD_LOADSTORE_INT(v_uint16x16, uint16_t)
SIMD_LOADSTORE_INT(v_int16x16,  int16_t)
SIMD_LOADSTORE_INT(v_uint32x8,  uint32_t)
SIMD_LOADSTORE_INT(v_int32x8,   int32_t)
SIMD_LOADSTORE_INT(v_uint64x4,  uint64_t)
SIMD_LOADSTORE_INT(v_int64x4,   int64_t)

#undef SIMD_LOADSTORE_INT

#define SIMD_LOADSTORE_FLT(_Tpvec, _Tp, suffix, halfreg)                \
    inline _Tpvec v256_load(const _Tp* ptr)                             \
    { return _Tpvec(_mm256_loadu_##suffix(ptr)); }                      \
    inline _Tpvec v256_load_aligned(const _Tp* ptr)                     \
    { return _Tpvec(_mm256_load_##suffix(ptr)); }                       \
    inline _Tpvec v256_load_low(const _Tp* ptr)                         \
    {                                                                   \
        return _Tpvec(_mm256_cast##suffix##128_##suffix##256            \
                (_mm_loadu_##suffix(ptr)));                             \
    }                                                                   \
    inline _Tpvec v256_load_halves(const _Tp* ptr0, const _Tp* ptr1)    \
    {                                                                   \
        halfreg vlo = _mm_loadu_##suffix(ptr0);                         \
        halfreg vhi = _mm_loadu_##suffix(ptr1);                         \
        return _Tpvec(_v256_combine(vlo, vhi));                         \
    }                                                                   \
    inline void v_store(_Tp* ptr, const _Tpvec& a)                      \
    { _mm256_storeu_##suffix(ptr, a.val); }                             \
    inline void v_store_aligned(_Tp* ptr, const _Tpvec& a)              \
    { _mm256_store_##suffix(ptr, a.val); }                              \
    inline void v_store_aligned_nocache(_Tp* ptr, const _Tpvec& a)      \
    { _mm256_stream_##suffix(ptr, a.val); }                             \
    inline void v_store(_Tp* ptr, const _Tpvec& a, StoreMode mode)      \
    {                                                                   \
        if (mode == StoreMode::ADDR_UNALIGNED)                               \
            _mm256_storeu_##suffix(ptr, a.val);                         \
        else if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)                    \
            _mm256_stream_##suffix(ptr, a.val);                         \
        else                                                            \
            _mm256_store_##suffix(ptr, a.val);                          \
    }                                                                   \
    inline void v_store_low(_Tp* ptr, const _Tpvec& a)                  \
    { _mm_storeu_##suffix(ptr, _v256_extract_low(a.val)); }             \
    inline void v_store_high(_Tp* ptr, const _Tpvec& a)                 \
    { _mm_storeu_##suffix(ptr, _v256_extract_high(a.val)); }

SIMD_LOADSTORE_FLT(v_float32x8, float,  ps, __m128)
SIMD_LOADSTORE_FLT(v_float64x4, double, pd, __m128d)

#undef SIMD_LOADSTORE_FLT

#define SIMD_CAST(_Tpvec, _Tpvecf, suffix, cast)                        \
    inline _Tpvec v_reinterpret_as_##suffix(const _Tpvecf& a)           \
    { return _Tpvec(cast(a.val)); }

#define SIMD_INIT_INT(_Tpvec, _Tp, suffix, ssuffix, ctype_s)            \
    inline _Tpvec v256_setzero_##suffix()                               \
    { return _Tpvec(_mm256_setzero_si256()); }                          \
    inline _Tpvec v256_setall_##suffix(_Tp v)                           \
    { return _Tpvec(_mm256_set1_##ssuffix((ctype_s)v)); }               \
    SIMD_CAST(_Tpvec, v_uint8x32,  suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_int8x32,   suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_uint16x16, suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_int16x16,  suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_uint32x8,  suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_int32x8,   suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_uint64x4,  suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_int64x4,   suffix, SIMD_NOP)                    \
    SIMD_CAST(_Tpvec, v_float32x8, suffix, _mm256_castps_si256)         \
    SIMD_CAST(_Tpvec, v_float64x4, suffix, _mm256_castpd_si256)

SIMD_INIT_INT(v_uint8x32,  uint8_t,    u8,  epi8,   int8_t)
SIMD_INIT_INT(v_int8x32,   int8_t,     s8,  epi8,   int8_t)
SIMD_INIT_INT(v_uint16x16, uint16_t,   u16, epi16,  int16_t)
SIMD_INIT_INT(v_int16x16,  int16_t,    s16, epi16,  int16_t)
SIMD_INIT_INT(v_uint32x8,  uint32_t,   u32, epi32,  int32_t)
SIMD_INIT_INT(v_int32x8,   int32_t,    s32, epi32,  int32_t)
SIMD_INIT_INT(v_uint64x4,  uint64_t,   u64, epi64x, int64_t)
SIMD_INIT_INT(v_int64x4,   int64_t,    s64, epi64x, int64_t)

#undef SIMD_INIT

#define SIMD_INIT_FLT(_Tpvec, _Tp, suffix, zsuffix, cast)               \
    inline _Tpvec v256_setzero_##suffix()                               \
    { return _Tpvec(_mm256_setzero_##zsuffix()); }                      \
    inline _Tpvec v256_setall_##suffix(_Tp v)                           \
    { return _Tpvec(_mm256_set1_##zsuffix(v)); }                        \
    SIMD_CAST(_Tpvec, v_uint8x32,  suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_int8x32,   suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_uint16x16, suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_int16x16,  suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_uint32x8,  suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_int32x8,   suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_uint64x4,  suffix, cast)                        \
    SIMD_CAST(_Tpvec, v_int64x4,   suffix, cast)

SIMD_INIT_FLT(v_float32x8, float,  f32, ps, _mm256_castsi256_ps)
SIMD_INIT_FLT(v_float64x4, double, f64, pd, _mm256_castsi256_pd)

#undef SIMD_INIT_FLT

inline v_float32x8 v_reinterpret_as_f32(const v_float32x8& a)
{ return a; }
inline v_float32x8 v_reinterpret_as_f32(const v_float64x4& a)
{ return v_float32x8(_mm256_castpd_ps(a.val)); }

inline v_float64x4 v_reinterpret_as_f64(const v_float64x4& a)
{ return a; }
inline v_float64x4 v_reinterpret_as_f64(const v_float32x8& a)
{ return v_float64x4(_mm256_castps_pd(a.val)); }

//////////////// Variant Value reordering ///////////////

// unpacks
#define SIMD_UNPACK(_Tpvec, suffix)                                 \
    inline _Tpvec v256_unpacklo(const _Tpvec& a, const _Tpvec& b)   \
    { return _Tpvec(_mm256_unpacklo_##suffix(a.val, b.val)); }      \
    inline _Tpvec v256_unpackhi(const _Tpvec& a, const _Tpvec& b)   \
    { return _Tpvec(_mm256_unpackhi_##suffix(a.val, b.val)); }

SIMD_UNPACK(v_uint8x32,  epi8)
SIMD_UNPACK(v_int8x32,   epi8)
SIMD_UNPACK(v_uint16x16, epi16)
SIMD_UNPACK(v_int16x16,  epi16)
SIMD_UNPACK(v_uint32x8,  epi32)
SIMD_UNPACK(v_int32x8,   epi32)
SIMD_UNPACK(v_uint64x4,  epi64)
SIMD_UNPACK(v_int64x4,   epi64)
SIMD_UNPACK(v_float32x8, ps)
SIMD_UNPACK(v_float64x4, pd)

#undef SIMD_UNPACK

// blend
#define SIMD_BLEND(_Tpvec, suffix)                                  \
    template<int32_t m>                                             \
    inline _Tpvec v256_blend(const _Tpvec& a, const _Tpvec& b)      \
    { return _Tpvec(_mm256_blend_##suffix(a.val, b.val, m)); }

SIMD_BLEND(v_uint16x16, epi16)
SIMD_BLEND(v_int16x16,  epi16)
SIMD_BLEND(v_uint32x8,  epi32)
SIMD_BLEND(v_int32x8,   epi32)
SIMD_BLEND(v_float32x8, ps)
SIMD_BLEND(v_float64x4, pd)

#undef SIMD_BLEND

template<int32_t m>
inline v_uint64x4 v256_blend(const v_uint64x4& a, const v_uint64x4& b)
{
    enum {M0 = m};
    enum {M1 = (M0 | (M0 << 2)) & 0x33};
    enum {M2 = (M1 | (M1 << 1)) & 0x55};
    enum {MM =  M2 | (M2 << 1)};
    return v_uint64x4(_mm256_blend_epi32(a.val, b.val, MM));
}
template<int32_t m>
inline v_int64x4 v256_blend(const v_int64x4& a, const v_int64x4& b)
{ return v_int64x4(v256_blend<m>(v_uint64x4(a.val), v_uint64x4(b.val)).val); }

// shuffle
// todo: emulate 64bit
#define SIMD_SHUFFLE(_Tpvec, intrin)                                \
    template<int32_t m>                                             \
    inline _Tpvec v256_shuffle(const _Tpvec& a)                     \
    { return _Tpvec(_mm256_##intrin(a.val, m)); }

SIMD_SHUFFLE(v_uint32x8,  shuffle_epi32)
SIMD_SHUFFLE(v_int32x8,   shuffle_epi32)
SIMD_SHUFFLE(v_float32x8, permute_ps)
SIMD_SHUFFLE(v_float64x4, permute_pd)

#undef SIMD_SHUFFLE

template<typename _Tpvec>
inline void v256_zip(const _Tpvec& a, const _Tpvec& b, _Tpvec& ab0, _Tpvec& ab1)
{
    ab0 = v256_unpacklo(a, b);
    ab1 = v256_unpackhi(a, b);
}

template<typename _Tpvec>
inline _Tpvec v256_combine_diagonal(const _Tpvec& a, const _Tpvec& b)
{ return _Tpvec(_mm256_blend_epi32(a.val, b.val, 0xf0)); }

inline v_float32x8 v256_combine_diagonal(const v_float32x8& a, const v_float32x8& b)
{ return v256_blend<0xf0>(a, b); }

inline v_float64x4 v256_combine_diagonal(const v_float64x4& a, const v_float64x4& b)
{ return v256_blend<0xc>(a, b); }

template<typename _Tpvec>
inline _Tpvec v256_alignr_128(const _Tpvec& a, const _Tpvec& b)
{ return v256_permute2x128<0x21>(a, b); }

template<typename _Tpvec>
inline _Tpvec v256_alignr_64(const _Tpvec& a, const _Tpvec& b)
{ return _Tpvec(_mm256_alignr_epi8(a.val, b.val, 8)); }
inline v_float64x4 v256_alignr_64(const v_float64x4& a, const v_float64x4& b)
{ return v_float64x4(_mm256_shuffle_pd(b.val, a.val, _MM_SHUFFLE(0, 0, 1, 1))); }
// todo: emulate float32

template<typename _Tpvec>
inline _Tpvec v256_swap_halves(const _Tpvec& a)
{ return v256_permute2x128<1>(a, a); }

template<typename _Tpvec>
inline _Tpvec v256_reverse_64(const _Tpvec& a)
{ return v256_permute4x64<_MM_SHUFFLE(0, 1, 2, 3)>(a); }

// ZIP
#define SIMD_ZIP(_Tpvec)                                            \
    inline _Tpvec v_combine_low(const _Tpvec& a, const _Tpvec& b)   \
    { return v256_permute2x128<0x20>(a, b); }                       \
    inline _Tpvec v_combine_high(const _Tpvec& a, const _Tpvec& b)  \
    { return v256_permute2x128<0x31>(a, b); }                       \
    inline void v_recombine(const _Tpvec& a, const _Tpvec& b,       \
            _Tpvec& c, _Tpvec& d)                                   \
    {                                                               \
        _Tpvec a1b0 = v256_alignr_128(a, b);                        \
        c = v256_combine_diagonal(a, a1b0);                         \
        d = v256_combine_diagonal(a1b0, b);                         \
    }                                                               \
    inline void v_zip(const _Tpvec& a, const _Tpvec& b,             \
            _Tpvec& ab0, _Tpvec& ab1)                               \
    {                                                               \
        _Tpvec ab0ab2, ab1ab3;                                      \
        v256_zip(a, b, ab0ab2, ab1ab3);                             \
        v_recombine(ab0ab2, ab1ab3, ab0, ab1);                      \
    }

SIMD_ZIP(v_uint8x32)
SIMD_ZIP(v_int8x32)
SIMD_ZIP(v_uint16x16)
SIMD_ZIP(v_int16x16)
SIMD_ZIP(v_uint32x8)
SIMD_ZIP(v_int32x8)
SIMD_ZIP(v_uint64x4)
SIMD_ZIP(v_int64x4)
SIMD_ZIP(v_float32x8)
SIMD_ZIP(v_float64x4)

#undef SIMD_ZIP

////////// Arithmetic, bitwise and comparison operations /////////

/* Element-wise binary and unary operations */

/** Arithmetics **/
#define SIMD_BIN_OP(bin_op, _Tpvec, intrin)                         \
    inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b)\
    { return _Tpvec(intrin(a.val, b.val)); }                        \
    inline _Tpvec& operator bin_op##= (_Tpvec& a, const _Tpvec& b)  \
    { a.val = intrin(a.val, b.val); return a; }

SIMD_BIN_OP(+, v_uint8x32,  _mm256_adds_epu8)
SIMD_BIN_OP(-, v_uint8x32,  _mm256_subs_epu8)
SIMD_BIN_OP(+, v_int8x32,   _mm256_adds_epi8)
SIMD_BIN_OP(-, v_int8x32,   _mm256_subs_epi8)
SIMD_BIN_OP(+, v_uint16x16, _mm256_adds_epu16)
SIMD_BIN_OP(-, v_uint16x16, _mm256_subs_epu16)
SIMD_BIN_OP(+, v_int16x16,  _mm256_adds_epi16)
SIMD_BIN_OP(-, v_int16x16,  _mm256_subs_epi16)
SIMD_BIN_OP(+, v_uint32x8,  _mm256_add_epi32)
SIMD_BIN_OP(-, v_uint32x8,  _mm256_sub_epi32)
SIMD_BIN_OP(*, v_uint32x8,  _mm256_mullo_epi32)
SIMD_BIN_OP(+, v_int32x8,   _mm256_add_epi32)
SIMD_BIN_OP(-, v_int32x8,   _mm256_sub_epi32)
SIMD_BIN_OP(*, v_int32x8,   _mm256_mullo_epi32)
SIMD_BIN_OP(+, v_uint64x4,  _mm256_add_epi64)
SIMD_BIN_OP(-, v_uint64x4,  _mm256_sub_epi64)
SIMD_BIN_OP(+, v_int64x4,   _mm256_add_epi64)
SIMD_BIN_OP(-, v_int64x4,   _mm256_sub_epi64)

SIMD_BIN_OP(+, v_float32x8, _mm256_add_ps)
SIMD_BIN_OP(-, v_float32x8, _mm256_sub_ps)
SIMD_BIN_OP(*, v_float32x8, _mm256_mul_ps)
SIMD_BIN_OP(/, v_float32x8, _mm256_div_ps)
SIMD_BIN_OP(+, v_float64x4, _mm256_add_pd)
SIMD_BIN_OP(-, v_float64x4, _mm256_sub_pd)
SIMD_BIN_OP(*, v_float64x4, _mm256_mul_pd)
SIMD_BIN_OP(/, v_float64x4, _mm256_div_pd)

// forward declaration
void v_mul_expand(const v_uint8x32&, const v_uint8x32&, v_uint16x16&, v_uint16x16&);
void v_mul_expand(const v_int8x32&, const v_int8x32&, v_int16x16&, v_int16x16&);

v_uint8x32 v_pack(const v_uint16x16&, const v_uint16x16&);
v_int8x32  v_pack(const v_int16x16&, const v_int16x16&);

// saturating multiply 8-bit, 16-bit
inline v_uint8x32 operator * (const v_uint8x32& a, const v_uint8x32& b)
{
    v_uint16x16 c, d;
    v_mul_expand(a, b, c, d);
    return v_pack(c, d);
}
inline v_int8x32 operator * (const v_int8x32& a, const v_int8x32& b)
{
    v_int16x16 c, d;
    v_mul_expand(a, b, c, d);
    return v_pack(c, d);
}
inline v_uint16x16 operator * (const v_uint16x16& a, const v_uint16x16& b)
{
    __m256i pl = _mm256_mullo_epi16(a.val, b.val);
    __m256i ph = _mm256_mulhi_epu16(a.val, b.val);
    __m256i p0 = _mm256_unpacklo_epi16(pl, ph);
    __m256i p1 = _mm256_unpackhi_epi16(pl, ph);
    return v_uint16x16(_v256_packs_epu32(p0, p1));
}
inline v_int16x16 operator * (const v_int16x16& a, const v_int16x16& b)
{
    __m256i pl = _mm256_mullo_epi16(a.val, b.val);
    __m256i ph = _mm256_mulhi_epi16(a.val, b.val);
    __m256i p0 = _mm256_unpacklo_epi16(pl, ph);
    __m256i p1 = _mm256_unpackhi_epi16(pl, ph);
    return v_int16x16(_mm256_packs_epi32(p0, p1));
}
inline v_uint8x32& operator *= (v_uint8x32& a, const v_uint8x32& b)
{ a = a * b; return a; }
inline v_int8x32& operator *= (v_int8x32& a, const v_int8x32& b)
{ a = a * b; return a; }
inline v_uint16x16& operator *= (v_uint16x16& a, const v_uint16x16& b)
{ a = a * b; return a; }
inline v_int16x16& operator *= (v_int16x16& a, const v_int16x16& b)
{ a = a * b; return a; }

/** Non-saturating arithmetics **/
#define SIMD_BIN_FUNC(func, _Tpvec, intrin)                         \
    inline _Tpvec func(const _Tpvec& a, const _Tpvec& b)            \
    { return _Tpvec(intrin(a.val, b.val)); }

SIMD_BIN_FUNC(v_add_wrap, v_uint8x32,  _mm256_add_epi8)
SIMD_BIN_FUNC(v_add_wrap, v_int8x32,   _mm256_add_epi8)
SIMD_BIN_FUNC(v_add_wrap, v_uint16x16, _mm256_add_epi16)
SIMD_BIN_FUNC(v_add_wrap, v_int16x16,  _mm256_add_epi16)
SIMD_BIN_FUNC(v_sub_wrap, v_uint8x32,  _mm256_sub_epi8)
SIMD_BIN_FUNC(v_sub_wrap, v_int8x32,   _mm256_sub_epi8)
SIMD_BIN_FUNC(v_sub_wrap, v_uint16x16, _mm256_sub_epi16)
SIMD_BIN_FUNC(v_sub_wrap, v_int16x16,  _mm256_sub_epi16)
SIMD_BIN_FUNC(v_mul_wrap, v_uint16x16, _mm256_mullo_epi16)
SIMD_BIN_FUNC(v_mul_wrap, v_int16x16,  _mm256_mullo_epi16)

inline v_uint8x32 v_mul_wrap(const v_uint8x32& a, const v_uint8x32& b)
{
    __m256i ad = _mm256_srai_epi16(a.val, 8);
    __m256i bd = _mm256_srai_epi16(b.val, 8);
    __m256i p0 = _mm256_mullo_epi16(a.val, b.val); // even
    __m256i p1 = _mm256_slli_epi16(_mm256_mullo_epi16(ad, bd), 8); // odd

    const __m256i b01 = _mm256_set1_epi32(0xFF00FF00);
    return v_uint8x32(_mm256_blendv_epi8(p0, p1, b01));
}
inline v_int8x32 v_mul_wrap(const v_int8x32& a, const v_int8x32& b)
{
    return v_reinterpret_as_s8(v_mul_wrap(v_reinterpret_as_u8(a), v_reinterpret_as_u8(b)));
}

void v_expand(const v_uint8x32&, v_uint16x16&, v_uint16x16&);
void v_expand(const v_int8x32&, v_int16x16&, v_int16x16&);

//  Multiply and expand
inline void v_mul_expand(const v_uint8x32& a, const v_uint8x32& b,
                         v_uint16x16& c, v_uint16x16& d)
{
    v_uint16x16 a0, a1, b0, b1;
    v_expand(a, a0, a1);
    v_expand(b, b0, b1);
    c = v_mul_wrap(a0, b0);
    d = v_mul_wrap(a1, b1);
}

inline void v_mul_expand(const v_int8x32& a, const v_int8x32& b,
                         v_int16x16& c, v_int16x16& d)
{
    v_int16x16 a0, a1, b0, b1;
    v_expand(a, a0, a1);
    v_expand(b, b0, b1);
    c = v_mul_wrap(a0, b0);
    d = v_mul_wrap(a1, b1);
}

inline void v_mul_expand(const v_int16x16& a, const v_int16x16& b,
                         v_int32x8& c, v_int32x8& d)
{
    v_int16x16 vhi = v_int16x16(_mm256_mulhi_epi16(a.val, b.val));

    v_int16x16 v0, v1;
    v_zip(v_mul_wrap(a, b), vhi, v0, v1);

    c = v_reinterpret_as_s32(v0);
    d = v_reinterpret_as_s32(v1);
}

inline void v_mul_expand(const v_uint16x16& a, const v_uint16x16& b,
                         v_uint32x8& c, v_uint32x8& d)
{
    v_uint16x16 vhi = v_uint16x16(_mm256_mulhi_epu16(a.val, b.val));

    v_uint16x16 v0, v1;
    v_zip(v_mul_wrap(a, b), vhi, v0, v1);

    c = v_reinterpret_as_u32(v0);
    d = v_reinterpret_as_u32(v1);
}

inline void v_mul_expand(const v_uint32x8& a, const v_uint32x8& b,
                         v_uint64x4& c, v_uint64x4& d)
{
    __m256i v0 = _mm256_mul_epu32(a.val, b.val);
    __m256i v1 = _mm256_mul_epu32(_mm256_srli_epi64(a.val, 32), _mm256_srli_epi64(b.val, 32));
    v_zip(v_uint64x4(v0), v_uint64x4(v1), c, d);
}

inline v_int16x16 v_mul_hi(const v_int16x16& a, const v_int16x16& b) { return v_int16x16(_mm256_mulhi_epi16(a.val, b.val)); }
inline v_uint16x16 v_mul_hi(const v_uint16x16& a, const v_uint16x16& b) { return v_uint16x16(_mm256_mulhi_epu16(a.val, b.val)); }

inline __m256i _mm256_srai_epi64xx(const __m256i a, int32_t imm)
{
    __m256i d = _mm256_set1_epi64x((int64_t)1 << 63);
    __m256i r = _mm256_srli_epi64(_mm256_add_epi64(a, d), imm);
    return _mm256_sub_epi64(r, _mm256_srli_epi64(d, imm));
}

/** Bitwise shifts **/
#define SIMD_SHIFT_OP(_Tpuvec, _Tpsvec, suffix, srai)               \
    inline _Tpuvec operator << (const _Tpuvec& a, int32_t imm)      \
    { return _Tpuvec(_mm256_slli_##suffix(a.val, imm)); }           \
    inline _Tpsvec operator << (const _Tpsvec& a, int32_t imm)      \
    { return _Tpsvec(_mm256_slli_##suffix(a.val, imm)); }           \
    inline _Tpuvec operator >> (const _Tpuvec& a, int32_t imm)      \
    { return _Tpuvec(_mm256_srli_##suffix(a.val, imm)); }           \
    inline _Tpsvec operator >> (const _Tpsvec& a, int32_t imm)      \
    { return _Tpsvec(srai(a.val, imm)); }                           \
    template<int32_t imm>                                           \
    inline _Tpuvec v_shl(const _Tpuvec& a)                          \
    { return _Tpuvec(_mm256_slli_##suffix(a.val, imm)); }           \
    template<int32_t imm>                                           \
    inline _Tpsvec v_shl(const _Tpsvec& a)                          \
    { return _Tpsvec(_mm256_slli_##suffix(a.val, imm)); }           \
    template<int32_t imm>                                           \
    inline _Tpuvec v_shr(const _Tpuvec& a)                          \
    { return _Tpuvec(_mm256_srli_##suffix(a.val, imm)); }           \
    template<int32_t imm>                                           \
    inline _Tpsvec v_shr(const _Tpsvec& a)                          \
    { return _Tpsvec(srai(a.val, imm)); }

SIMD_SHIFT_OP(v_uint16x16, v_int16x16, epi16, _mm256_srai_epi16)
SIMD_SHIFT_OP(v_uint32x8,  v_int32x8,  epi32, _mm256_srai_epi32)
SIMD_SHIFT_OP(v_uint64x4,  v_int64x4,  epi64, _mm256_srai_epi64xx)

/** Bitwise logic **/
#define SIMD_LOGIC_OP(_Tpvec, suffix, not_const)                    \
    SIMD_BIN_OP(&, _Tpvec, _mm256_and_##suffix)                     \
    SIMD_BIN_OP(|, _Tpvec, _mm256_or_##suffix)                      \
    SIMD_BIN_OP(^, _Tpvec, _mm256_xor_##suffix)                     \
    inline _Tpvec operator ~ (const _Tpvec& a)                      \
    { return _Tpvec(_mm256_xor_##suffix(a.val, not_const)); }

SIMD_LOGIC_OP(v_uint8x32,   si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_int8x32,    si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_uint16x16,  si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_int16x16,   si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_uint32x8,   si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_int32x8,    si256, _mm256_set1_epi32(-1))
SIMD_LOGIC_OP(v_uint64x4,   si256, _mm256_set1_epi64x(-1))
SIMD_LOGIC_OP(v_int64x4,    si256, _mm256_set1_epi64x(-1))
SIMD_LOGIC_OP(v_float32x8,  ps,    _mm256_castsi256_ps(_mm256_set1_epi32(-1)))
SIMD_LOGIC_OP(v_float64x4,  pd,    _mm256_castsi256_pd(_mm256_set1_epi32(-1)))

/** Select **/
#define SIMD_SELECT(_Tpvec, suffix)                                 \
    inline _Tpvec v_select(const _Tpvec& mask, const _Tpvec& a, const _Tpvec& b) \
    { return _Tpvec(_mm256_blendv_##suffix(b.val, a.val, mask.val)); }

SIMD_SELECT(v_uint8x32,  epi8)
SIMD_SELECT(v_int8x32,   epi8)
SIMD_SELECT(v_uint16x16, epi8)
SIMD_SELECT(v_int16x16,  epi8)
SIMD_SELECT(v_uint32x8,  epi8)
SIMD_SELECT(v_int32x8,   epi8)
SIMD_SELECT(v_float32x8, ps)
SIMD_SELECT(v_float64x4, pd)

/** Comparison **/
#define SIMD_CMP_OP_OV(_Tpvec)                                      \
    inline _Tpvec operator != (const _Tpvec& a, const _Tpvec& b)    \
    { return ~(a == b); }                                           \
    inline _Tpvec operator <  (const _Tpvec& a, const _Tpvec& b)    \
    { return b > a; }                                               \
    inline _Tpvec operator >= (const _Tpvec& a, const _Tpvec& b)    \
    { return ~(a < b); }                                            \
    inline _Tpvec operator <= (const _Tpvec& a, const _Tpvec& b)    \
    { return b >= a; }

#define SIMD_CMP_OP_INT(_Tpuvec, _Tpsvec, suffix, sbit)             \
    inline _Tpuvec operator == (const _Tpuvec& a, const _Tpuvec& b) \
    { return _Tpuvec(_mm256_cmpeq_##suffix(a.val, b.val)); }        \
    inline _Tpuvec operator > (const _Tpuvec& a, const _Tpuvec& b)  \
    {                                                               \
        __m256i smask = _mm256_set1_##suffix(sbit);                 \
        return _Tpuvec(_mm256_cmpgt_##suffix(                       \
                       _mm256_xor_si256(a.val, smask),              \
                       _mm256_xor_si256(b.val, smask)));            \
    }                                                               \
    inline _Tpsvec operator == (const _Tpsvec& a, const _Tpsvec& b) \
    { return _Tpsvec(_mm256_cmpeq_##suffix(a.val, b.val)); }        \
    inline _Tpsvec operator > (const _Tpsvec& a, const _Tpsvec& b)  \
    { return _Tpsvec(_mm256_cmpgt_##suffix(a.val, b.val)); }        \
    SIMD_CMP_OP_OV(_Tpuvec)                                         \
    SIMD_CMP_OP_OV(_Tpsvec)

SIMD_CMP_OP_INT(v_uint8x32,  v_int8x32,  epi8,  (int8_t)-128)
SIMD_CMP_OP_INT(v_uint16x16, v_int16x16, epi16, (int16_t)-32768)
SIMD_CMP_OP_INT(v_uint32x8,  v_int32x8,  epi32, (int32_t)0x80000000)

#define SIMD_OP_64BIT(_Tpvec)                                       \
    inline _Tpvec operator == (const _Tpvec& a, const _Tpvec& b)    \
    { return _Tpvec(_mm256_cmpeq_epi64(a.val, b.val)); }            \
    inline _Tpvec operator != (const _Tpvec& a, const _Tpvec& b)    \
    { return ~(a == b); }

SIMD_OP_64BIT(v_uint64x4)
SIMD_OP_64BIT(v_int64x4)

#define SIMD_CMP_FLT(bin_op, imm8, _Tpvec, suffix)                  \
    inline _Tpvec operator bin_op (const _Tpvec& a, const _Tpvec& b)\
    { return _Tpvec(_mm256_cmp_##suffix(a.val, b.val, imm8)); }

#define SIMD_CMP_OP_FLT(_Tpvec, suffix)                             \
    SIMD_CMP_FLT(==, _CMP_EQ_OQ,  _Tpvec, suffix)                   \
    SIMD_CMP_FLT(!=, _CMP_NEQ_OQ, _Tpvec, suffix)                   \
    SIMD_CMP_FLT(<,  _CMP_LT_OQ,  _Tpvec, suffix)                   \
    SIMD_CMP_FLT(>,  _CMP_GT_OQ,  _Tpvec, suffix)                   \
    SIMD_CMP_FLT(<=, _CMP_LE_OQ,  _Tpvec, suffix)                   \
    SIMD_CMP_FLT(>=, _CMP_GE_OQ,  _Tpvec, suffix)

SIMD_CMP_OP_FLT(v_float32x8, ps)
SIMD_CMP_OP_FLT(v_float64x4, pd)

inline v_float32x8 v_not_nan(const v_float32x8& a)
{ return v_float32x8(_mm256_cmp_ps(a.val, a.val, _CMP_ORD_Q)); }
inline v_float64x4 v_not_nan(const v_float64x4& a)
{ return v_float64x4(_mm256_cmp_pd(a.val, a.val, _CMP_ORD_Q)); }

/** min/max **/
SIMD_BIN_FUNC(v_min, v_uint8x32,  _mm256_min_epu8)
SIMD_BIN_FUNC(v_max, v_uint8x32,  _mm256_max_epu8)
SIMD_BIN_FUNC(v_min, v_int8x32,   _mm256_min_epi8)
SIMD_BIN_FUNC(v_max, v_int8x32,   _mm256_max_epi8)
SIMD_BIN_FUNC(v_min, v_uint16x16, _mm256_min_epu16)
SIMD_BIN_FUNC(v_max, v_uint16x16, _mm256_max_epu16)
SIMD_BIN_FUNC(v_min, v_int16x16,  _mm256_min_epi16)
SIMD_BIN_FUNC(v_max, v_int16x16,  _mm256_max_epi16)
SIMD_BIN_FUNC(v_min, v_uint32x8,  _mm256_min_epu32)
SIMD_BIN_FUNC(v_max, v_uint32x8,  _mm256_max_epu32)
SIMD_BIN_FUNC(v_min, v_int32x8,   _mm256_min_epi32)
SIMD_BIN_FUNC(v_max, v_int32x8,   _mm256_max_epi32)
SIMD_BIN_FUNC(v_min, v_float32x8, _mm256_min_ps)
SIMD_BIN_FUNC(v_max, v_float32x8, _mm256_max_ps)
SIMD_BIN_FUNC(v_min, v_float64x4, _mm256_min_pd)
SIMD_BIN_FUNC(v_max, v_float64x4, _mm256_max_pd)

/** Rotate **/
template<int32_t imm>
inline v_uint8x32 v_rotate_left(const v_uint8x32& a, const v_uint8x32& b)
{
    enum {IMM_R = (16 - imm) & 0xFF};
    enum {IMM_R2 = (32 - imm) & 0xFF};

    if (imm == 0)  return a;
    if (imm == 32) return b;
    if (imm > 32)  return v_uint8x32();

    __m256i swap = _mm256_permute2x128_si256(a.val, b.val, 0x03);
    if (imm == 16) return v_uint8x32(swap);
    if (imm < 16)  return v_uint8x32(_mm256_alignr_epi8(a.val, swap, IMM_R));
    return v_uint8x32(_mm256_alignr_epi8(swap, b.val, IMM_R2)); // imm < 32
}

template<int32_t imm>
inline v_uint8x32 v_rotate_right(const v_uint8x32& a, const v_uint8x32& b)
{
    enum {IMM_L = (imm - 16) & 0xFF};

    if (imm == 0)  return a;
    if (imm == 32) return b;
    if (imm > 32)  return v_uint8x32();

    __m256i swap = _mm256_permute2x128_si256(a.val, b.val, 0x21);
    if (imm == 16) return v_uint8x32(swap);
    if (imm < 16)  return v_uint8x32(_mm256_alignr_epi8(swap, a.val, imm));
    return v_uint8x32(_mm256_alignr_epi8(b.val, swap, IMM_L));
}

template<int32_t imm>
inline v_uint8x32 v_rotate_left(const v_uint8x32& a)
{
    enum {IMM_L = (imm - 16) & 0xFF};
    enum {IMM_R = (16 - imm) & 0xFF};

    if (imm == 0) return a;
    if (imm > 32) return v_uint8x32();

    // ESAC control[3] ? [127:0] = 0
    __m256i swapz = _mm256_permute2x128_si256(a.val, a.val, _MM_SHUFFLE(0, 0, 2, 0));
    if (imm == 16) return v_uint8x32(swapz);
    if (imm < 16)  return v_uint8x32(_mm256_alignr_epi8(a.val, swapz, IMM_R));
    return v_uint8x32(_mm256_slli_si256(swapz, IMM_L));
}

template<int32_t imm>
inline v_uint8x32 v_rotate_right(const v_uint8x32& a)
{
    enum {IMM_L = (imm - 16) & 0xFF};

    if (imm == 0) return a;
    if (imm > 32) return v_uint8x32();

    // ESAC control[3] ? [127:0] = 0
    __m256i swapz = _mm256_permute2x128_si256(a.val, a.val, _MM_SHUFFLE(2, 0, 0, 1));
    if (imm == 16) return v_uint8x32(swapz);
    if (imm < 16)  return v_uint8x32(_mm256_alignr_epi8(swapz, a.val, imm));
    return v_uint8x32(_mm256_srli_si256(swapz, IMM_L));
}

#define SIMD_ROTATE_CAST(intrin, _Tpvec, cast)                      \
    template<int32_t imm>                                           \
    inline _Tpvec intrin(const _Tpvec& a, const _Tpvec& b)          \
    {                                                               \
        enum {IMMxW = imm * sizeof(typename _Tpvec::lane_type)};    \
        v_uint8x32 ret = intrin<IMMxW>(v_reinterpret_as_u8(a),      \
                                       v_reinterpret_as_u8(b));     \
        return _Tpvec(cast(ret.val));                               \
    }                                                               \
    template<int32_t imm>                                           \
    inline _Tpvec intrin(const _Tpvec& a)                           \
    {                                                               \
        enum {IMMxW = imm * sizeof(typename _Tpvec::lane_type)};    \
        v_uint8x32 ret = intrin<IMMxW>(v_reinterpret_as_u8(a));     \
        return _Tpvec(cast(ret.val));                               \
    }

#define SIMD_ROTATE(_Tpvec)                                         \
    SIMD_ROTATE_CAST(v_rotate_left,  _Tpvec, SIMD_NOP)              \
    SIMD_ROTATE_CAST(v_rotate_right, _Tpvec, SIMD_NOP)

SIMD_ROTATE(v_int8x32)
SIMD_ROTATE(v_uint16x16)
SIMD_ROTATE(v_int16x16)
SIMD_ROTATE(v_uint32x8)
SIMD_ROTATE(v_int32x8)
SIMD_ROTATE(v_uint64x4)
SIMD_ROTATE(v_int64x4)

SIMD_ROTATE_CAST(v_rotate_left,  v_float32x8, _mm256_castsi256_ps)
SIMD_ROTATE_CAST(v_rotate_right, v_float32x8, _mm256_castsi256_ps)
SIMD_ROTATE_CAST(v_rotate_left,  v_float64x4, _mm256_castsi256_pd)
SIMD_ROTATE_CAST(v_rotate_right, v_float64x4, _mm256_castsi256_pd)

/** Reverse **/
inline v_uint8x32 v_reverse(const v_uint8x32 &a)
{
    static const __m256i perm = _mm256_setr_epi8(
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    __m256i vec = _mm256_shuffle_epi8(a.val, perm);
    return v_uint8x32(_mm256_permute2x128_si256(vec, vec, 1));
}

inline v_int8x32 v_reverse(const v_int8x32 &a)
{ return v_reinterpret_as_s8(v_reverse(v_reinterpret_as_u8(a))); }

inline v_uint16x16 v_reverse(const v_uint16x16 &a)
{
    static const __m256i perm = _mm256_setr_epi8(
            14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1,
            14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
    __m256i vec = _mm256_shuffle_epi8(a.val, perm);
    return v_uint16x16(_mm256_permute2x128_si256(vec, vec, 1));
}

inline v_int16x16 v_reverse(const v_int16x16 &a)
{ return v_reinterpret_as_s16(v_reverse(v_reinterpret_as_u16(a))); }

inline v_uint32x8 v_reverse(const v_uint32x8 &a)
{
    static const __m256i perm = _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0);
    return v_uint32x8(_mm256_permutevar8x32_epi32(a.val, perm));
}

inline v_int32x8 v_reverse(const v_int32x8 &a)
{ return v_reinterpret_as_s32(v_reverse(v_reinterpret_as_u32(a))); }

inline v_float32x8 v_reverse(const v_float32x8 &a)
{ return v_reinterpret_as_f32(v_reverse(v_reinterpret_as_u32(a))); }

inline v_uint64x4 v_reverse(const v_uint64x4 &a)
{
    return v_uint64x4(_mm256_permute4x64_epi64(a.val, _MM_SHUFFLE(0, 1, 2, 3)));
}

inline v_int64x4 v_reverse(const v_int64x4 &a)
{ return v_reinterpret_as_s64(v_reverse(v_reinterpret_as_u64(a))); }

inline v_float64x4 v_reverse(const v_float64x4 &a)
{ return v_reinterpret_as_f64(v_reverse(v_reinterpret_as_u64(a))); }

////////// Reduce and mask /////////

/** Reduce **/
inline uint32_t v_reduce_sum(const v_uint8x32& a)
{
    __m256i half = _mm256_sad_epu8(a.val, _mm256_setzero_si256());
    __m128i quarter = _mm_add_epi32(_v256_extract_low(half), _v256_extract_high(half));
    return (uint32_t)_mm_cvtsi128_si32(_mm_add_epi32(quarter, _mm_unpackhi_epi64(quarter, quarter)));
}
inline int32_t v_reduce_sum(const v_int8x32& a)
{
    __m256i half = _mm256_sad_epu8(_mm256_xor_si256(a.val, _mm256_set1_epi8((int8_t)-128)), _mm256_setzero_si256());
    __m128i quarter = _mm_add_epi32(_v256_extract_low(half), _v256_extract_high(half));
    return (uint32_t)_mm_cvtsi128_si32(_mm_add_epi32(quarter, _mm_unpackhi_epi64(quarter, quarter))) - 4096;
}

#define SIMD_REDUCE_32(_Tpvec, sctype, func, intrin)                \
    inline sctype v_reduce_##func(const _Tpvec& a)                  \
    {                                                               \
        __m128i val = intrin(_v256_extract_low(a.val), _v256_extract_high(a.val)); \
        val = intrin(val, _mm_srli_si128(val,8));                   \
        val = intrin(val, _mm_srli_si128(val,4));                   \
        val = intrin(val, _mm_srli_si128(val,2));                   \
        val = intrin(val, _mm_srli_si128(val,1));                   \
        return (sctype)_mm_cvtsi128_si32(val);                      \
    }

SIMD_REDUCE_32(v_uint8x32, uint8_t, min, _mm_min_epu8)
SIMD_REDUCE_32(v_int8x32,  int8_t,  min, _mm_min_epi8)
SIMD_REDUCE_32(v_uint8x32, uint8_t, max, _mm_max_epu8)
SIMD_REDUCE_32(v_int8x32,  int8_t,  max, _mm_max_epi8)

#define SIMD_REDUCE_16(_Tpvec, sctype, func, intrin)                \
    inline sctype v_reduce_##func(const _Tpvec& a)                  \
    {                                                               \
        __m128i v0 = _v256_extract_low(a.val);                      \
        __m128i v1 = _v256_extract_high(a.val);                     \
        v0 = intrin(v0, v1);                                        \
        v0 = intrin(v0, _mm_srli_si128(v0, 8));                     \
        v0 = intrin(v0, _mm_srli_si128(v0, 4));                     \
        v0 = intrin(v0, _mm_srli_si128(v0, 2));                     \
        return (sctype) _mm_cvtsi128_si32(v0);                      \
    }

SIMD_REDUCE_16(v_uint16x16, uint16_t, min, _mm_min_epu16)
SIMD_REDUCE_16(v_int16x16,  int16_t,  min, _mm_min_epi16)
SIMD_REDUCE_16(v_uint16x16, uint16_t, max, _mm_max_epu16)
SIMD_REDUCE_16(v_int16x16,  int16_t,  max, _mm_max_epi16)

#define SIMD_REDUCE_8(_Tpvec, sctype, func, intrin)                 \
    inline sctype v_reduce_##func(const _Tpvec& a)                  \
    {                                                               \
        __m128i v0 = _v256_extract_low(a.val);                      \
        __m128i v1 = _v256_extract_high(a.val);                     \
        v0 = intrin(v0, v1);                                        \
        v0 = intrin(v0, _mm_srli_si128(v0, 8));                     \
        v0 = intrin(v0, _mm_srli_si128(v0, 4));                     \
        return (sctype) _mm_cvtsi128_si32(v0);                      \
    }

SIMD_REDUCE_8(v_uint32x8, uint32_t, min, _mm_min_epu32)
SIMD_REDUCE_8(v_int32x8,  int32_t,  min, _mm_min_epi32)
SIMD_REDUCE_8(v_uint32x8, uint32_t, max, _mm_max_epu32)
SIMD_REDUCE_8(v_int32x8,  int32_t,  max, _mm_max_epi32)

#define SIMD_REDUCE_FLT(func, intrin)                               \
    inline float v_reduce_##func(const v_float32x8& a)              \
    {                                                               \
        __m128 v0 = _v256_extract_low(a.val);                       \
        __m128 v1 = _v256_extract_high(a.val);                      \
        v0 = intrin(v0, v1);                                        \
        v0 = intrin(v0, _mm_permute_ps(v0, _MM_SHUFFLE(0, 0, 3, 2))); \
        v0 = intrin(v0, _mm_permute_ps(v0, _MM_SHUFFLE(0, 0, 0, 1))); \
        return _mm_cvtss_f32(v0);                                   \
    }

SIMD_REDUCE_FLT(min, _mm_min_ps)
SIMD_REDUCE_FLT(max, _mm_max_ps)

inline int32_t v_reduce_sum(const v_int32x8& a)
{
    __m256i s0 = _mm256_hadd_epi32(a.val, a.val);
            s0 = _mm256_hadd_epi32(s0, s0);

    __m128i s1 = _v256_extract_high(s0);
            s1 = _mm_add_epi32(_v256_extract_low(s0), s1);

    return _mm_cvtsi128_si32(s1);
}

inline uint32_t v_reduce_sum(const v_uint32x8& a)
{ return v_reduce_sum(v_reinterpret_as_s32(a)); }

v_int32x8 v_expand_low(const v_int16x16&);
v_uint32x8 v_expand_low(const v_uint16x16&);
v_int32x8 v_expand_high(const v_int16x16&);
v_uint32x8 v_expand_high(const v_uint16x16&);

inline int32_t v_reduce_sum(const v_int16x16& a)
{ return v_reduce_sum(v_expand_low(a) + v_expand_high(a)); }
inline uint32_t v_reduce_sum(const v_uint16x16& a)
{ return v_reduce_sum(v_expand_low(a) + v_expand_high(a)); }

inline float v_reduce_sum(const v_float32x8& a)
{
    __m256 s0 = _mm256_hadd_ps(a.val, a.val);
    s0 = _mm256_hadd_ps(s0, s0);

    __m128 s1 = _v256_extract_high(s0);
    s1 = _mm_add_ps(_v256_extract_low(s0), s1);

    return _mm_cvtss_f32(s1);
}

inline uint64_t v_reduce_sum(const v_uint64x4& a)
{
    uint64_t DECL_ALIGNED(32) idx[2];
    _mm_store_si128((__m128i*)idx, _mm_add_epi64(_v256_extract_low(a.val), _v256_extract_high(a.val)));
    return idx[0] + idx[1];
}
inline int64_t v_reduce_sum(const v_int64x4& a)
{
    int64_t DECL_ALIGNED(32) idx[2];
    _mm_store_si128((__m128i*)idx, _mm_add_epi64(_v256_extract_low(a.val), _v256_extract_high(a.val)));
    return idx[0] + idx[1];
}
inline double v_reduce_sum(const v_float64x4& a)
{
    __m256d s0 = _mm256_hadd_pd(a.val, a.val);
    return _mm_cvtsd_f64(_mm_add_pd(_v256_extract_low(s0), _v256_extract_high(s0)));
}

inline v_float32x8 v_reduce_sum4(const v_float32x8& a, const v_float32x8& b, const v_float32x8& c, const v_float32x8& d)
{
    __m256 ab = _mm256_hadd_ps(a.val, b.val);
    __m256 cd = _mm256_hadd_ps(c.val, d.val);
    return v_float32x8(_mm256_hadd_ps(ab, cd));
}

inline uint32_t v_reduce_sad(const v_uint8x32& a, const v_uint8x32& b)
{
    __m256i half = _mm256_sad_epu8(a.val, b.val);
    __m128i quarter = _mm_add_epi32(_v256_extract_low(half), _v256_extract_high(half));
    return (uint32_t)_mm_cvtsi128_si32(_mm_add_epi32(quarter, _mm_unpackhi_epi64(quarter, quarter)));
}
inline uint32_t v_reduce_sad(const v_int8x32& a, const v_int8x32& b)
{
    __m256i half = _mm256_set1_epi8(0x7f);
    half = _mm256_sad_epu8(_mm256_add_epi8(a.val, half), _mm256_add_epi8(b.val, half));
    __m128i quarter = _mm_add_epi32(_v256_extract_low(half), _v256_extract_high(half));
    return (uint32_t)_mm_cvtsi128_si32(_mm_add_epi32(quarter, _mm_unpackhi_epi64(quarter, quarter)));
}

void v_expand(const v_uint16x16&, v_uint32x8&, v_uint32x8&);
inline uint32_t v_reduce_sad(const v_uint16x16& a, const v_uint16x16& b)
{
    v_uint32x8 l, h;
    v_expand(v_add_wrap(a - b, b - a), l, h);
    return v_reduce_sum(l + h);
}
inline uint32_t v_reduce_sad(const v_int16x16& a, const v_int16x16& b)
{
    v_uint32x8 l, h;
    v_expand(v_reinterpret_as_u16(v_sub_wrap(v_max(a, b), v_min(a, b))), l, h);
    return v_reduce_sum(l + h);
}
inline uint32_t v_reduce_sad(const v_uint32x8& a, const v_uint32x8& b)
{
    return v_reduce_sum(v_max(a, b) - v_min(a, b));
}
inline uint32_t v_reduce_sad(const v_int32x8& a, const v_int32x8& b)
{
    v_int32x8 m = a < b;
    return v_reduce_sum(v_reinterpret_as_u32(((a - b) ^ m) - m));
}
inline float v_reduce_sad(const v_float32x8& a, const v_float32x8& b)
{
    return v_reduce_sum((a - b) & v_float32x8(_mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff))));
}

/** Popcount **/
inline v_uint8x32 v_popcount(const v_uint8x32& a)
{
    __m256i _popcnt_table = _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                                             0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    __m256i _popcnt_mask = _mm256_set1_epi8(0x0F);
    return v_uint8x32(_mm256_add_epi8(_mm256_shuffle_epi8(_popcnt_table, _mm256_and_si256(                  a.val    , _popcnt_mask)),
                                      _mm256_shuffle_epi8(_popcnt_table, _mm256_and_si256(_mm256_srli_epi16(a.val, 4), _popcnt_mask))));
}
inline v_uint16x16 v_popcount(const v_uint16x16& a)
{
    v_uint8x32 p = v_popcount(v_reinterpret_as_u8(a));
    p += v_rotate_right<1>(p);
    return v_reinterpret_as_u16(p) & v256_setall_u16(0x00ff);
}
inline v_uint32x8 v_popcount(const v_uint32x8& a)
{
    v_uint8x32 p = v_popcount(v_reinterpret_as_u8(a));
    p += v_rotate_right<1>(p);
    p += v_rotate_right<2>(p);
    return v_reinterpret_as_u32(p) & v256_setall_u32(0x000000ff);
}
inline v_uint64x4 v_popcount(const v_uint64x4& a)
{
    return v_uint64x4(_mm256_sad_epu8(v_popcount(v_reinterpret_as_u8(a)).val, _mm256_setzero_si256()));
}
inline v_uint8x32 v_popcount(const v_int8x32& a)
{ return v_popcount(v_reinterpret_as_u8(a)); }
inline v_uint16x16 v_popcount(const v_int16x16& a)
{ return v_popcount(v_reinterpret_as_u16(a)); }
inline v_uint32x8 v_popcount(const v_int32x8& a)
{ return v_popcount(v_reinterpret_as_u32(a)); }
inline v_uint64x4 v_popcount(const v_int64x4& a)
{ return v_popcount(v_reinterpret_as_u64(a)); }

/** Mask **/
inline int32_t v_signmask(const v_int8x32& a)
{ return _mm256_movemask_epi8(a.val); }
inline int32_t v_signmask(const v_uint8x32& a)
{ return v_signmask(v_reinterpret_as_s8(a)); }

inline int32_t v_signmask(const v_int16x16& a)
{ return v_signmask(v_pack(a, a)) & 0xFFFF; }
inline int32_t v_signmask(const v_uint16x16& a)
{ return v_signmask(v_reinterpret_as_s16(a)); }

inline int32_t v_signmask(const v_float32x8& a)
{ return _mm256_movemask_ps(a.val); }
inline int32_t v_signmask(const v_float64x4& a)
{ return _mm256_movemask_pd(a.val); }

inline int32_t v_signmask(const v_int32x8& a)
{ return v_signmask(v_reinterpret_as_f32(a)); }
inline int32_t v_signmask(const v_uint32x8& a)
{ return v_signmask(v_reinterpret_as_f32(a)); }

inline int32_t v_signmask(const v_int64x4& a)
{ return v_signmask(v_reinterpret_as_f64(a)); }
inline int32_t v_signmask(const v_uint64x4& a)
{ return v_signmask(v_reinterpret_as_f64(a)); }

inline int32_t v_scan_forward(const v_int8x32& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))); }
inline int32_t v_scan_forward(const v_uint8x32& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))); }
inline int32_t v_scan_forward(const v_int16x16& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 2; }
inline int32_t v_scan_forward(const v_uint16x16& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 2; }
inline int32_t v_scan_forward(const v_int32x8& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 4; }
inline int32_t v_scan_forward(const v_uint32x8& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 4; }
inline int32_t v_scan_forward(const v_float32x8& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 4; }
inline int32_t v_scan_forward(const v_int64x4& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 8; }
inline int32_t v_scan_forward(const v_uint64x4& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 8; }
inline int32_t v_scan_forward(const v_float64x4& a) { return trailingZeros32(v_signmask(v_reinterpret_as_s8(a))) / 8; }

/** Checks **/
#define SIMD_CHECK(_Tpvec, allmask)                                 \
    inline bool v_check_all(const _Tpvec& a) { return v_signmask(a) == allmask; } \
    inline bool v_check_any(const _Tpvec& a) { return v_signmask(a) != 0; }

SIMD_CHECK(v_uint8x32,  -1)
SIMD_CHECK(v_int8x32,   -1)
SIMD_CHECK(v_uint32x8,  255)
SIMD_CHECK(v_int32x8,   255)
SIMD_CHECK(v_uint64x4,  15)
SIMD_CHECK(v_int64x4,   15)
SIMD_CHECK(v_float32x8, 255)
SIMD_CHECK(v_float64x4, 15)

#define SIMD_CHECK_SHORT(_Tpvec)                                    \
    inline bool v_check_all(const _Tpvec& a) { return (v_signmask(v_reinterpret_as_s8(a)) & 0xaaaaaaaa) == 0xaaaaaaaa; } \
    inline bool v_check_any(const _Tpvec& a) { return (v_signmask(v_reinterpret_as_s8(a)) & 0xaaaaaaaa) != 0; }
SIMD_CHECK_SHORT(v_uint16x16)
SIMD_CHECK_SHORT(v_int16x16)

////////// Other math /////////

/** Some frequent operations **/
#if SIMD_USE_FMA3
#define SIMD_MULADD(_Tpvec, suffix)                                             \
    inline _Tpvec v_fma(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c)      \
    { return _Tpvec(_mm256_fmadd_##suffix(a.val, b.val, c.val)); }              \
    inline _Tpvec v_muladd(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c)   \
    { return _Tpvec(_mm256_fmadd_##suffix(a.val, b.val, c.val)); }
#else
#define SIMD_MULADD(_Tpvec, suffix)                                                     \
    inline _Tpvec v_fma(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c)              \
    { return _Tpvec(_mm256_add_##suffix(_mm256_mul_##suffix(a.val, b.val), c.val)); }   \
    inline _Tpvec v_muladd(const _Tpvec& a, const _Tpvec& b, const _Tpvec& c)           \
    { return _Tpvec(_mm256_add_##suffix(_mm256_mul_##suffix(a.val, b.val), c.val)); }
#endif

#define SIMD_MISC(_Tpvec, suffix)                              \
    inline _Tpvec v_sqrt(const _Tpvec& x)                                     \
    { return _Tpvec(_mm256_sqrt_##suffix(x.val)); }                           \
    inline _Tpvec v_sqr_magnitude(const _Tpvec& a, const _Tpvec& b)           \
    { return v_fma(a, a, b * b); }                                            \
    inline _Tpvec v_magnitude(const _Tpvec& a, const _Tpvec& b)               \
    { return v_sqrt(v_fma(a, a, b*b)); }

SIMD_MULADD(v_float32x8, ps)
SIMD_MULADD(v_float64x4, pd)
SIMD_MISC(v_float32x8, ps)
SIMD_MISC(v_float64x4, pd)

inline v_int32x8 v_fma(const v_int32x8& a, const v_int32x8& b, const v_int32x8& c)
{
    return a * b + c;
}

inline v_int32x8 v_muladd(const v_int32x8& a, const v_int32x8& b, const v_int32x8& c)
{
    return v_fma(a, b, c);
}

inline v_float32x8 v_invsqrt(const v_float32x8& x)
{
    v_float32x8 half = x * v256_setall_f32(0.5);
    v_float32x8 t  = v_float32x8(_mm256_rsqrt_ps(x.val));
    // todo: _mm256_fnmsub_ps
    t *= v256_setall_f32(1.5) - ((t * t) * half);
    return t;
}

inline v_float64x4 v_invsqrt(const v_float64x4& x)
{
    return v256_setall_f64(1.) / v_sqrt(x);
}

/** Absolute values **/
#define SIMD_ABS(_Tpvec, suffix)                                    \
    inline v_u##_Tpvec v_abs(const v_##_Tpvec& x)                   \
    { return v_u##_Tpvec(_mm256_abs_##suffix(x.val)); }

SIMD_ABS(int8x32,  epi8)
SIMD_ABS(int16x16, epi16)
SIMD_ABS(int32x8,  epi32)

inline v_float32x8 v_abs(const v_float32x8& x)
{ return x & v_float32x8(_mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff))); }
inline v_float64x4 v_abs(const v_float64x4& x)
{ return x & v_float64x4(_mm256_castsi256_pd(_mm256_srli_epi64(_mm256_set1_epi64x(-1), 1))); }

/** Absolute difference **/
inline v_uint8x32 v_absdiff(const v_uint8x32& a, const v_uint8x32& b)
{ return v_add_wrap(a - b,  b - a); }
inline v_uint16x16 v_absdiff(const v_uint16x16& a, const v_uint16x16& b)
{ return v_add_wrap(a - b,  b - a); }
inline v_uint32x8 v_absdiff(const v_uint32x8& a, const v_uint32x8& b)
{ return v_max(a, b) - v_min(a, b); }

inline v_uint8x32 v_absdiff(const v_int8x32& a, const v_int8x32& b)
{
    v_int8x32 d = v_sub_wrap(a, b);
    v_int8x32 m = a < b;
    return v_reinterpret_as_u8(v_sub_wrap(d ^ m, m));
}

inline v_uint16x16 v_absdiff(const v_int16x16& a, const v_int16x16& b)
{ return v_reinterpret_as_u16(v_sub_wrap(v_max(a, b), v_min(a, b))); }

inline v_uint32x8 v_absdiff(const v_int32x8& a, const v_int32x8& b)
{
    v_int32x8 d = a - b;
    v_int32x8 m = a < b;
    return v_reinterpret_as_u32((d ^ m) - m);
}

inline v_float32x8 v_absdiff(const v_float32x8& a, const v_float32x8& b)
{ return v_abs(a - b); }

inline v_float64x4 v_absdiff(const v_float64x4& a, const v_float64x4& b)
{ return v_abs(a - b); }

/** Saturating absolute difference **/
inline v_int8x32 v_absdiffs(const v_int8x32& a, const v_int8x32& b)
{
    v_int8x32 d = a - b;
    v_int8x32 m = a < b;
    return (d ^ m) - m;
}
inline v_int16x16 v_absdiffs(const v_int16x16& a, const v_int16x16& b)
{ return v_max(a, b) - v_min(a, b); }

////////// Conversions /////////

/** Rounding **/
inline v_int32x8 v_round(const v_float32x8& a)
{ return v_int32x8(_mm256_cvtps_epi32(a.val)); }

inline v_int32x8 v_round(const v_float64x4& a)
{ return v_int32x8(_mm256_castsi128_si256(_mm256_cvtpd_epi32(a.val))); }

inline v_int32x8 v_round(const v_float64x4& a, const v_float64x4& b)
{
    __m128i ai = _mm256_cvtpd_epi32(a.val), bi = _mm256_cvtpd_epi32(b.val);
    return v_int32x8(_v256_combine(ai, bi));
}

inline v_int32x8 v_trunc(const v_float32x8& a)
{ return v_int32x8(_mm256_cvttps_epi32(a.val)); }

inline v_int32x8 v_trunc(const v_float64x4& a)
{ return v_int32x8(_mm256_castsi128_si256(_mm256_cvttpd_epi32(a.val))); }

inline v_int32x8 v_floor(const v_float32x8& a)
{ return v_int32x8(_mm256_cvttps_epi32(_mm256_floor_ps(a.val))); }

inline v_int32x8 v_floor(const v_float64x4& a)
{ return v_trunc(v_float64x4(_mm256_floor_pd(a.val))); }

inline v_int32x8 v_ceil(const v_float32x8& a)
{ return v_int32x8(_mm256_cvttps_epi32(_mm256_ceil_ps(a.val))); }

inline v_int32x8 v_ceil(const v_float64x4& a)
{ return v_trunc(v_float64x4(_mm256_ceil_pd(a.val))); }

/** To float **/
inline v_float32x8 v_cvt_f32(const v_int32x8& a)
{ return v_float32x8(_mm256_cvtepi32_ps(a.val)); }

inline v_float32x8 v_cvt_f32(const v_float64x4& a)
{ return v_float32x8(_mm256_castps128_ps256(_mm256_cvtpd_ps(a.val))); }

inline v_float32x8 v_cvt_f32(const v_float64x4& a, const v_float64x4& b)
{
    __m128 af = _mm256_cvtpd_ps(a.val), bf = _mm256_cvtpd_ps(b.val);
    return v_float32x8(_v256_combine(af, bf));
}

inline v_float64x4 v_cvt_f64(const v_int32x8& a)
{ return v_float64x4(_mm256_cvtepi32_pd(_v256_extract_low(a.val))); }

inline v_float64x4 v_cvt_f64_high(const v_int32x8& a)
{ return v_float64x4(_mm256_cvtepi32_pd(_v256_extract_high(a.val))); }

inline v_float64x4 v_cvt_f64(const v_float32x8& a)
{ return v_float64x4(_mm256_cvtps_pd(_v256_extract_low(a.val))); }

inline v_float64x4 v_cvt_f64_high(const v_float32x8& a)
{ return v_float64x4(_mm256_cvtps_pd(_v256_extract_high(a.val))); }

// from (Mysticial and wim) https://stackoverflow.com/q/41144668
inline v_float64x4 v_cvt_f64(const v_int64x4& v)
{
    // constants encoded as floating-point
    __m256i magic_i_lo   = _mm256_set1_epi64x(0x4330000000000000); // 2^52
    __m256i magic_i_hi32 = _mm256_set1_epi64x(0x4530000080000000); // 2^84 + 2^63
    __m256i magic_i_all  = _mm256_set1_epi64x(0x4530000080100000); // 2^84 + 2^63 + 2^52
    __m256d magic_d_all  = _mm256_castsi256_pd(magic_i_all);

    // Blend the 32 lowest significant bits of v with magic_int_lo
    __m256i v_lo         = _mm256_blend_epi32(magic_i_lo, v.val, 0x55);
    // Extract the 32 most significant bits of v
    __m256i v_hi         = _mm256_srli_epi64(v.val, 32);
    // Flip the msb of v_hi and blend with 0x45300000
            v_hi         = _mm256_xor_si256(v_hi, magic_i_hi32);
    // Compute in double precision
    __m256d v_hi_dbl     = _mm256_sub_pd(_mm256_castsi256_pd(v_hi), magic_d_all);
    // (v_hi - magic_d_all) + v_lo  Do not assume associativity of floating point addition
    __m256d result       = _mm256_add_pd(v_hi_dbl, _mm256_castsi256_pd(v_lo));
    return v_float64x4(result);
}

////////////// Lookup table access ////////////////////

inline v_int8x32 v256_lut(const int8_t* tab, const int32_t* idx)
{
    return v_int8x32(_mm256_setr_epi8(tab[idx[ 0]], tab[idx[ 1]], tab[idx[ 2]], tab[idx[ 3]], tab[idx[ 4]], tab[idx[ 5]], tab[idx[ 6]], tab[idx[ 7]],
                                      tab[idx[ 8]], tab[idx[ 9]], tab[idx[10]], tab[idx[11]], tab[idx[12]], tab[idx[13]], tab[idx[14]], tab[idx[15]],
                                      tab[idx[16]], tab[idx[17]], tab[idx[18]], tab[idx[19]], tab[idx[20]], tab[idx[21]], tab[idx[22]], tab[idx[23]],
                                      tab[idx[24]], tab[idx[25]], tab[idx[26]], tab[idx[27]], tab[idx[28]], tab[idx[29]], tab[idx[30]], tab[idx[31]]));
}
inline v_int8x32 v256_lut_pairs(const int8_t* tab, const int32_t* idx)
{
    return v_int8x32(_mm256_setr_epi16(*(const int16_t*)(tab + idx[ 0]), *(const int16_t*)(tab + idx[ 1]), *(const int16_t*)(tab + idx[ 2]), *(const int16_t*)(tab + idx[ 3]),
                                       *(const int16_t*)(tab + idx[ 4]), *(const int16_t*)(tab + idx[ 5]), *(const int16_t*)(tab + idx[ 6]), *(const int16_t*)(tab + idx[ 7]),
                                       *(const int16_t*)(tab + idx[ 8]), *(const int16_t*)(tab + idx[ 9]), *(const int16_t*)(tab + idx[10]), *(const int16_t*)(tab + idx[11]),
                                       *(const int16_t*)(tab + idx[12]), *(const int16_t*)(tab + idx[13]), *(const int16_t*)(tab + idx[14]), *(const int16_t*)(tab + idx[15])));
}
inline v_int8x32 v256_lut_quads(const int8_t* tab, const int32_t* idx)
{
    return v_int8x32(_mm256_i32gather_epi32((const int32_t*)tab, _mm256_loadu_si256((const __m256i*)idx), 1));
}
inline v_uint8x32 v256_lut(const uint8_t* tab, const int32_t* idx) { return v_reinterpret_as_u8(v256_lut((const int8_t *)tab, idx)); }
inline v_uint8x32 v256_lut_pairs(const uint8_t* tab, const int32_t* idx) { return v_reinterpret_as_u8(v256_lut_pairs((const int8_t *)tab, idx)); }
inline v_uint8x32 v256_lut_quads(const uint8_t* tab, const int32_t* idx) { return v_reinterpret_as_u8(v256_lut_quads((const int8_t *)tab, idx)); }

inline v_int16x16 v256_lut(const int16_t* tab, const int32_t* idx)
{
    return v_int16x16(_mm256_setr_epi16(tab[idx[0]], tab[idx[1]], tab[idx[ 2]], tab[idx[ 3]], tab[idx[ 4]], tab[idx[ 5]], tab[idx[ 6]], tab[idx[ 7]],
                                        tab[idx[8]], tab[idx[9]], tab[idx[10]], tab[idx[11]], tab[idx[12]], tab[idx[13]], tab[idx[14]], tab[idx[15]]));
}
inline v_int16x16 v256_lut_pairs(const int16_t* tab, const int32_t* idx)
{
    return v_int16x16(_mm256_i32gather_epi32((const int32_t*)tab, _mm256_loadu_si256((const __m256i*)idx), 2));
}
inline v_int16x16 v256_lut_quads(const int16_t* tab, const int32_t* idx)
{
#if defined(__GNUC__) && !defined(__clang__)
    return v_int16x16(_mm256_i32gather_epi64((const long long int32_t*)tab, _mm_loadu_si128((const __m128i*)idx), 2));//Looks like intrinsic has wrong definition
#else
    return v_int16x16(_mm256_i32gather_epi64((const int64_t*)tab, _mm_loadu_si128((const __m128i*)idx), 2));
#endif
}
inline v_uint16x16 v256_lut(const uint16_t* tab, const int32_t* idx) { return v_reinterpret_as_u16(v256_lut((const int16_t *)tab, idx)); }
inline v_uint16x16 v256_lut_pairs(const uint16_t* tab, const int32_t* idx) { return v_reinterpret_as_u16(v256_lut_pairs((const int16_t *)tab, idx)); }
inline v_uint16x16 v256_lut_quads(const uint16_t* tab, const int32_t* idx) { return v_reinterpret_as_u16(v256_lut_quads((const int16_t *)tab, idx)); }

inline v_int32x8 v256_lut(const int32_t* tab, const int32_t* idx)
{
    return v_int32x8(_mm256_i32gather_epi32(tab, _mm256_loadu_si256((const __m256i*)idx), 4));
}
inline v_int32x8 v256_lut_pairs(const int32_t* tab, const int32_t* idx)
{
#if defined(__GNUC__) && !defined(__clang__)
    return v_int32x8(_mm256_i32gather_epi64((const long long int32_t*)tab, _mm_loadu_si128((const __m128i*)idx), 4));
#else
    return v_int32x8(_mm256_i32gather_epi64((const int64_t*)tab, _mm_loadu_si128((const __m128i*)idx), 4));
#endif
}
inline v_int32x8 v256_lut_quads(const int32_t* tab, const int32_t* idx)
{
    return v_int32x8(_v256_combine(_mm_loadu_si128((const __m128i*)(tab + idx[0])), _mm_loadu_si128((const __m128i*)(tab + idx[1]))));
}
inline v_uint32x8 v256_lut(const uint32_t* tab, const int32_t* idx) { return v_reinterpret_as_u32(v256_lut((const int32_t *)tab, idx)); }
inline v_uint32x8 v256_lut_pairs(const uint32_t* tab, const int32_t* idx) { return v_reinterpret_as_u32(v256_lut_pairs((const int32_t *)tab, idx)); }
inline v_uint32x8 v256_lut_quads(const uint32_t* tab, const int32_t* idx) { return v_reinterpret_as_u32(v256_lut_quads((const int32_t *)tab, idx)); }

inline v_int64x4 v256_lut(const int64_t* tab, const int32_t* idx)
{
#if defined(__GNUC__) && !defined(__clang__)
    return v_int64x4(_mm256_i32gather_epi64((const long long int32_t*)tab, _mm_loadu_si128((const __m128i*)idx), 8));
#else
    return v_int64x4(_mm256_i32gather_epi64(tab, _mm_loadu_si128((const __m128i*)idx), 8));
#endif
}
inline v_int64x4 v256_lut_pairs(const int64_t* tab, const int32_t* idx)
{
    return v_int64x4(_v256_combine(_mm_loadu_si128((const __m128i*)(tab + idx[0])), _mm_loadu_si128((const __m128i*)(tab + idx[1]))));
}
inline v_uint64x4 v256_lut(const uint64_t* tab, const int32_t* idx) { return v_reinterpret_as_u64(v256_lut((const int64_t *)tab, idx)); }
inline v_uint64x4 v256_lut_pairs(const uint64_t* tab, const int32_t* idx) { return v_reinterpret_as_u64(v256_lut_pairs((const int64_t *)tab, idx)); }

inline v_float32x8 v256_lut(const float* tab, const int32_t* idx)
{
    return v_float32x8(_mm256_i32gather_ps(tab, _mm256_loadu_si256((const __m256i*)idx), 4));
}
inline v_float32x8 v256_lut_pairs(const float* tab, const int32_t* idx) { return v_reinterpret_as_f32(v256_lut_pairs((const int32_t *)tab, idx)); }
inline v_float32x8 v256_lut_quads(const float* tab, const int32_t* idx) { return v_reinterpret_as_f32(v256_lut_quads((const int32_t *)tab, idx)); }

inline v_float64x4 v256_lut(const double* tab, const int32_t* idx)
{
    return v_float64x4(_mm256_i32gather_pd(tab, _mm_loadu_si128((const __m128i*)idx), 8));
}
inline v_float64x4 v256_lut_pairs(const double* tab, const int32_t* idx) { return v_float64x4(_v256_combine(_mm_loadu_pd(tab + idx[0]), _mm_loadu_pd(tab + idx[1]))); }

inline v_int32x8 v_lut(const int32_t* tab, const v_int32x8& idxvec)
{
    return v_int32x8(_mm256_i32gather_epi32(tab, idxvec.val, 4));
}

inline v_uint32x8 v_lut(const uint32_t* tab, const v_int32x8& idxvec)
{
    return v_reinterpret_as_u32(v_lut((const int32_t *)tab, idxvec));
}

inline v_float32x8 v_lut(const float* tab, const v_int32x8& idxvec)
{
    return v_float32x8(_mm256_i32gather_ps(tab, idxvec.val, 4));
}

inline v_float64x4 v_lut(const double* tab, const v_int32x8& idxvec)
{
    return v_float64x4(_mm256_i32gather_pd(tab, _mm256_castsi256_si128(idxvec.val), 8));
}

inline void v_lut_deinterleave(const float* tab, const v_int32x8& idxvec, v_float32x8& x, v_float32x8& y)
{
    int32_t DECL_ALIGNED(32) idx[8];
    v_store_aligned(idx, idxvec);
    __m128 z = _mm_setzero_ps();
    __m128 xy01, xy45, xy23, xy67;
    xy01 = _mm_loadl_pi(z, (const __m64*)(tab + idx[0]));
    xy01 = _mm_loadh_pi(xy01, (const __m64*)(tab + idx[1]));
    xy45 = _mm_loadl_pi(z, (const __m64*)(tab + idx[4]));
    xy45 = _mm_loadh_pi(xy45, (const __m64*)(tab + idx[5]));
    __m256 xy0145 = _v256_combine(xy01, xy45);
    xy23 = _mm_loadl_pi(z, (const __m64*)(tab + idx[2]));
    xy23 = _mm_loadh_pi(xy23, (const __m64*)(tab + idx[3]));
    xy67 = _mm_loadl_pi(z, (const __m64*)(tab + idx[6]));
    xy67 = _mm_loadh_pi(xy67, (const __m64*)(tab + idx[7]));
    __m256 xy2367 = _v256_combine(xy23, xy67);

    __m256 xxyy0145 = _mm256_unpacklo_ps(xy0145, xy2367);
    __m256 xxyy2367 = _mm256_unpackhi_ps(xy0145, xy2367);

    x = v_float32x8(_mm256_unpacklo_ps(xxyy0145, xxyy2367));
    y = v_float32x8(_mm256_unpackhi_ps(xxyy0145, xxyy2367));
}

inline void v_lut_deinterleave(const double* tab, const v_int32x8& idxvec, v_float64x4& x, v_float64x4& y)
{
    int32_t DECL_ALIGNED(32) idx[4];
    v_store_low(idx, idxvec);
    __m128d xy0 = _mm_loadu_pd(tab + idx[0]);
    __m128d xy2 = _mm_loadu_pd(tab + idx[2]);
    __m128d xy1 = _mm_loadu_pd(tab + idx[1]);
    __m128d xy3 = _mm_loadu_pd(tab + idx[3]);
    __m256d xy02 = _v256_combine(xy0, xy2);
    __m256d xy13 = _v256_combine(xy1, xy3);

    x = v_float64x4(_mm256_unpacklo_pd(xy02, xy13));
    y = v_float64x4(_mm256_unpackhi_pd(xy02, xy13));
}

inline v_int8x32 v_interleave_pairs(const v_int8x32& vec)
{
    return v_int8x32(_mm256_shuffle_epi8(vec.val, _mm256_set_epi64x(0x0f0d0e0c0b090a08, 0x0705060403010200, 0x0f0d0e0c0b090a08, 0x0705060403010200)));
}
inline v_uint8x32 v_interleave_pairs(const v_uint8x32& vec) { return v_reinterpret_as_u8(v_interleave_pairs(v_reinterpret_as_s8(vec))); }
inline v_int8x32 v_interleave_quads(const v_int8x32& vec)
{
    return v_int8x32(_mm256_shuffle_epi8(vec.val, _mm256_set_epi64x(0x0f0b0e0a0d090c08, 0x0703060205010400, 0x0f0b0e0a0d090c08, 0x0703060205010400)));
}
inline v_uint8x32 v_interleave_quads(const v_uint8x32& vec) { return v_reinterpret_as_u8(v_interleave_quads(v_reinterpret_as_s8(vec))); }

inline v_int16x16 v_interleave_pairs(const v_int16x16& vec)
{
    return v_int16x16(_mm256_shuffle_epi8(vec.val, _mm256_set_epi64x(0x0f0e0b0a0d0c0908, 0x0706030205040100, 0x0f0e0b0a0d0c0908, 0x0706030205040100)));
}
inline v_uint16x16 v_interleave_pairs(const v_uint16x16& vec) { return v_reinterpret_as_u16(v_interleave_pairs(v_reinterpret_as_s16(vec))); }
inline v_int16x16 v_interleave_quads(const v_int16x16& vec)
{
    return v_int16x16(_mm256_shuffle_epi8(vec.val, _mm256_set_epi64x(0x0f0e07060d0c0504, 0x0b0a030209080100, 0x0f0e07060d0c0504, 0x0b0a030209080100)));
}
inline v_uint16x16 v_interleave_quads(const v_uint16x16& vec) { return v_reinterpret_as_u16(v_interleave_quads(v_reinterpret_as_s16(vec))); }

inline v_int32x8 v_interleave_pairs(const v_int32x8& vec)
{
    return v_int32x8(_mm256_shuffle_epi32(vec.val, _MM_SHUFFLE(3, 1, 2, 0)));
}
inline v_uint32x8 v_interleave_pairs(const v_uint32x8& vec) { return v_reinterpret_as_u32(v_interleave_pairs(v_reinterpret_as_s32(vec))); }
inline v_float32x8 v_interleave_pairs(const v_float32x8& vec) { return v_reinterpret_as_f32(v_interleave_pairs(v_reinterpret_as_s32(vec))); }

inline v_int8x32 v_pack_triplets(const v_int8x32& vec)
{
    return v_int8x32(_mm256_permutevar8x32_epi32(_mm256_shuffle_epi8(vec.val, _mm256_broadcastsi128_si256(_mm_set_epi64x(0xffffff0f0e0d0c0a, 0x0908060504020100))),
                                                 _mm256_set_epi64x(0x0000000700000007, 0x0000000600000005, 0x0000000400000002, 0x0000000100000000)));
}
inline v_uint8x32 v_pack_triplets(const v_uint8x32& vec) { return v_reinterpret_as_u8(v_pack_triplets(v_reinterpret_as_s8(vec))); }

inline v_int16x16 v_pack_triplets(const v_int16x16& vec)
{
    return v_int16x16(_mm256_permutevar8x32_epi32(_mm256_shuffle_epi8(vec.val, _mm256_broadcastsi128_si256(_mm_set_epi64x(0xffff0f0e0d0c0b0a, 0x0908050403020100))),
                                                  _mm256_set_epi64x(0x0000000700000007, 0x0000000600000005, 0x0000000400000002, 0x0000000100000000)));
}
inline v_uint16x16 v_pack_triplets(const v_uint16x16& vec) { return v_reinterpret_as_u16(v_pack_triplets(v_reinterpret_as_s16(vec))); }

inline v_int32x8 v_pack_triplets(const v_int32x8& vec)
{
    return v_int32x8(_mm256_permutevar8x32_epi32(vec.val, _mm256_set_epi64x(0x0000000700000007, 0x0000000600000005, 0x0000000400000002, 0x0000000100000000)));
}
inline v_uint32x8 v_pack_triplets(const v_uint32x8& vec) { return v_reinterpret_as_u32(v_pack_triplets(v_reinterpret_as_s32(vec))); }
inline v_float32x8 v_pack_triplets(const v_float32x8& vec)
{
    return v_float32x8(_mm256_permutevar8x32_ps(vec.val, _mm256_set_epi64x(0x0000000700000007, 0x0000000600000005, 0x0000000400000002, 0x0000000100000000)));
}

inline void v_interleave(const v_int8x32& a0, const v_int8x32& a1, v_int8x32& b0, v_int8x32& b1)
{
    __m256i a_l = _mm256_unpacklo_epi8(a0.val, a1.val);
    __m256i a_h = _mm256_unpackhi_epi8(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_uint8x32& a0, const v_uint8x32& a1, v_uint8x32& b0, v_uint8x32& b1)
{
    __m256i a_l = _mm256_unpacklo_epi8(a0.val, a1.val);
    __m256i a_h = _mm256_unpackhi_epi8(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_int16x16& a0, const v_int16x16& a1, v_int16x16& b0, v_int16x16& b1)
{
    __m256i a_l = _mm256_unpacklo_epi16(a0.val, a1.val);
    __m256i a_h = _mm256_unpacklo_epi16(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_uint16x16& a0, const v_uint16x16& a1, v_uint16x16& b0, v_uint16x16& b1)
{
    __m256i a_l = _mm256_unpacklo_epi16(a0.val, a1.val);
    __m256i a_h = _mm256_unpacklo_epi16(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_int32x8& a0, const v_int32x8& a1, v_int32x8& b0, v_int32x8& b1)
{
    __m256i a_l = _mm256_unpacklo_epi32(a0.val, a1.val);
    __m256i a_h = _mm256_unpacklo_epi32(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_uint32x8& a0, const v_uint32x8& a1, v_uint32x8& b0, v_uint32x8& b1)
{
    __m256i a_l = _mm256_unpacklo_epi32(a0.val, a1.val);
    __m256i a_h = _mm256_unpacklo_epi32(a0.val, a1.val);
    b0.val = _mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = _mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

inline void v_interleave(const v_float32x8& a0, const v_float32x8& a1, v_float32x8& b0, v_float32x8& b1)
{
    __m256i a_l = _mm256_unpacklo_epi32((__m256i)a0.val, (__m256i)a1.val);
    __m256i a_h = _mm256_unpacklo_epi32((__m256i)a0.val, (__m256i)a1.val);
    b0.val = (__m256)_mm256_permute2x128_si256(a_l, a_h, 0+2*16);
    b1.val = (__m256)_mm256_permute2x128_si256(a_l, a_h, 1+3*16);
}

////////// Matrix operations /////////

//////// Dot Product ////////

// 16 >> 32
inline v_int32x8 v_dotprod(const v_int16x16& a, const v_int16x16& b)
{ return v_int32x8(_mm256_madd_epi16(a.val, b.val)); }
inline v_int32x8 v_dotprod(const v_int16x16& a, const v_int16x16& b, const v_int32x8& c)
{ return v_dotprod(a, b) + c; }

// 32 >> 64
inline v_int64x4 v_dotprod(const v_int32x8& a, const v_int32x8& b)
{
    __m256i even = _mm256_mul_epi32(a.val, b.val);
    __m256i odd = _mm256_mul_epi32(_mm256_srli_epi64(a.val, 32), _mm256_srli_epi64(b.val, 32));
    return v_int64x4(_mm256_add_epi64(even, odd));
}
inline v_int64x4 v_dotprod(const v_int32x8& a, const v_int32x8& b, const v_int64x4& c)
{ return v_dotprod(a, b) + c; }

// 8 >> 32
inline v_uint32x8 v_dotprod_expand(const v_uint8x32& a, const v_uint8x32& b)
{
    __m256i even_m = _mm256_set1_epi32(0xFF00FF00);
    __m256i even_a = _mm256_blendv_epi8(a.val, _mm256_setzero_si256(), even_m);
    __m256i odd_a  = _mm256_srli_epi16(a.val, 8);

    __m256i even_b = _mm256_blendv_epi8(b.val, _mm256_setzero_si256(), even_m);
    __m256i odd_b  = _mm256_srli_epi16(b.val, 8);

    __m256i prod0  = _mm256_madd_epi16(even_a, even_b);
    __m256i prod1  = _mm256_madd_epi16(odd_a, odd_b);
    return v_uint32x8(_mm256_add_epi32(prod0, prod1));
}
inline v_uint32x8 v_dotprod_expand(const v_uint8x32& a, const v_uint8x32& b, const v_uint32x8& c)
{ return v_dotprod_expand(a, b) + c; }

inline v_int32x8 v_dotprod_expand(const v_int8x32& a, const v_int8x32& b)
{
    __m256i even_a = _mm256_srai_epi16(_mm256_bslli_epi128(a.val, 1), 8);
    __m256i odd_a  = _mm256_srai_epi16(a.val, 8);

    __m256i even_b = _mm256_srai_epi16(_mm256_bslli_epi128(b.val, 1), 8);
    __m256i odd_b  = _mm256_srai_epi16(b.val, 8);

    __m256i prod0  = _mm256_madd_epi16(even_a, even_b);
    __m256i prod1  = _mm256_madd_epi16(odd_a, odd_b);
    return v_int32x8(_mm256_add_epi32(prod0, prod1));
}
inline v_int32x8 v_dotprod_expand(const v_int8x32& a, const v_int8x32& b, const v_int32x8& c)
{ return v_dotprod_expand(a, b) + c; }

// 16 >> 64
inline v_uint64x4 v_dotprod_expand(const v_uint16x16& a, const v_uint16x16& b)
{
    __m256i mullo = _mm256_mullo_epi16(a.val, b.val);
    __m256i mulhi = _mm256_mulhi_epu16(a.val, b.val);
    __m256i mul0  = _mm256_unpacklo_epi16(mullo, mulhi);
    __m256i mul1  = _mm256_unpackhi_epi16(mullo, mulhi);

    __m256i p02   = _mm256_blend_epi32(mul0, _mm256_setzero_si256(), 0xAA);
    __m256i p13   = _mm256_srli_epi64(mul0, 32);
    __m256i p46   = _mm256_blend_epi32(mul1, _mm256_setzero_si256(), 0xAA);
    __m256i p57   = _mm256_srli_epi64(mul1, 32);

    __m256i p15_  = _mm256_add_epi64(p02, p13);
    __m256i p9d_  = _mm256_add_epi64(p46, p57);

    return v_uint64x4(_mm256_add_epi64(
        _mm256_unpacklo_epi64(p15_, p9d_),
        _mm256_unpackhi_epi64(p15_, p9d_)
    ));
}
inline v_uint64x4 v_dotprod_expand(const v_uint16x16& a, const v_uint16x16& b, const v_uint64x4& c)
{ return v_dotprod_expand(a, b) + c; }

inline v_int64x4 v_dotprod_expand(const v_int16x16& a, const v_int16x16& b)
{
    __m256i prod = _mm256_madd_epi16(a.val, b.val);
    __m256i sign = _mm256_srai_epi32(prod, 31);

    __m256i lo = _mm256_unpacklo_epi32(prod, sign);
    __m256i hi = _mm256_unpackhi_epi32(prod, sign);

    return v_int64x4(_mm256_add_epi64(
        _mm256_unpacklo_epi64(lo, hi),
        _mm256_unpackhi_epi64(lo, hi)
    ));
}
inline v_int64x4 v_dotprod_expand(const v_int16x16& a, const v_int16x16& b, const v_int64x4& c)
{ return v_dotprod_expand(a, b) + c; }

// 32 >> 64f
inline v_float64x4 v_dotprod_expand(const v_int32x8& a, const v_int32x8& b)
{ return v_cvt_f64(v_dotprod(a, b)); }
inline v_float64x4 v_dotprod_expand(const v_int32x8& a, const v_int32x8& b, const v_float64x4& c)
{ return v_dotprod_expand(a, b) + c; }

//////// Fast Dot Product ////////

// 16 >> 32
inline v_int32x8 v_dotprod_fast(const v_int16x16& a, const v_int16x16& b)
{ return v_dotprod(a, b); }
inline v_int32x8 v_dotprod_fast(const v_int16x16& a, const v_int16x16& b, const v_int32x8& c)
{ return v_dotprod(a, b, c); }

// 32 >> 64
inline v_int64x4 v_dotprod_fast(const v_int32x8& a, const v_int32x8& b)
{ return v_dotprod(a, b); }
inline v_int64x4 v_dotprod_fast(const v_int32x8& a, const v_int32x8& b, const v_int64x4& c)
{ return v_dotprod(a, b, c); }

// 8 >> 32
inline v_uint32x8 v_dotprod_expand_fast(const v_uint8x32& a, const v_uint8x32& b)
{ return v_dotprod_expand(a, b); }
inline v_uint32x8 v_dotprod_expand_fast(const v_uint8x32& a, const v_uint8x32& b, const v_uint32x8& c)
{ return v_dotprod_expand(a, b, c); }

inline v_int32x8 v_dotprod_expand_fast(const v_int8x32& a, const v_int8x32& b)
{ return v_dotprod_expand(a, b); }
inline v_int32x8 v_dotprod_expand_fast(const v_int8x32& a, const v_int8x32& b, const v_int32x8& c)
{ return v_dotprod_expand(a, b, c); }

// 16 >> 64
inline v_uint64x4 v_dotprod_expand_fast(const v_uint16x16& a, const v_uint16x16& b)
{
    __m256i mullo = _mm256_mullo_epi16(a.val, b.val);
    __m256i mulhi = _mm256_mulhi_epu16(a.val, b.val);
    __m256i mul0  = _mm256_unpacklo_epi16(mullo, mulhi);
    __m256i mul1  = _mm256_unpackhi_epi16(mullo, mulhi);

    __m256i p02   = _mm256_blend_epi32(mul0, _mm256_setzero_si256(), 0xAA);
    __m256i p13   = _mm256_srli_epi64(mul0, 32);
    __m256i p46   = _mm256_blend_epi32(mul1, _mm256_setzero_si256(), 0xAA);
    __m256i p57   = _mm256_srli_epi64(mul1, 32);

    __m256i p15_  = _mm256_add_epi64(p02, p13);
    __m256i p9d_  = _mm256_add_epi64(p46, p57);

    return v_uint64x4(_mm256_add_epi64(p15_, p9d_));
}
inline v_uint64x4 v_dotprod_expand_fast(const v_uint16x16& a, const v_uint16x16& b, const v_uint64x4& c)
{ return v_dotprod_expand_fast(a, b) + c; }

inline v_int64x4 v_dotprod_expand_fast(const v_int16x16& a, const v_int16x16& b)
{
    __m256i prod = _mm256_madd_epi16(a.val, b.val);
    __m256i sign = _mm256_srai_epi32(prod, 31);
    __m256i lo = _mm256_unpacklo_epi32(prod, sign);
    __m256i hi = _mm256_unpackhi_epi32(prod, sign);
    return v_int64x4(_mm256_add_epi64(lo, hi));
}
inline v_int64x4 v_dotprod_expand_fast(const v_int16x16& a, const v_int16x16& b, const v_int64x4& c)
{ return v_dotprod_expand_fast(a, b) + c; }

// 32 >> 64f
inline v_float64x4 v_dotprod_expand_fast(const v_int32x8& a, const v_int32x8& b)
{ return v_dotprod_expand(a, b); }
inline v_float64x4 v_dotprod_expand_fast(const v_int32x8& a, const v_int32x8& b, const v_float64x4& c)
{ return v_dotprod_expand(a, b, c); }

#define SIMD_SPLAT2_PS(a, im)                                           \
    v_float32x8(_mm256_permute_ps(a.val, _MM_SHUFFLE(im, im, im, im)))

inline v_float32x8 v_matmul(const v_float32x8& v, const v_float32x8& m0,
                            const v_float32x8& m1, const v_float32x8& m2,
                            const v_float32x8& m3)
{
    v_float32x8 v04 = SIMD_SPLAT2_PS(v, 0);
    v_float32x8 v15 = SIMD_SPLAT2_PS(v, 1);
    v_float32x8 v26 = SIMD_SPLAT2_PS(v, 2);
    v_float32x8 v37 = SIMD_SPLAT2_PS(v, 3);
    return v_fma(v04, m0, v_fma(v15, m1, v_fma(v26, m2, v37 * m3)));
}

inline v_float32x8 v_matmuladd(const v_float32x8& v, const v_float32x8& m0,
                               const v_float32x8& m1, const v_float32x8& m2,
                               const v_float32x8& a)
{
    v_float32x8 v04 = SIMD_SPLAT2_PS(v, 0);
    v_float32x8 v15 = SIMD_SPLAT2_PS(v, 1);
    v_float32x8 v26 = SIMD_SPLAT2_PS(v, 2);
    return v_fma(v04, m0, v_fma(v15, m1, v_fma(v26, m2, a)));
}

#define SIMD_TRANSPOSE4x4(_Tpvec, suffix, cast_from, cast_to)                   \
    inline void v_transpose4x4(const _Tpvec& a0, const _Tpvec& a1,              \
                               const _Tpvec& a2, const _Tpvec& a3,              \
                               _Tpvec& b0, _Tpvec& b1, _Tpvec& b2, _Tpvec& b3)  \
    {                                                                           \
        __m256i t0 = cast_from(_mm256_unpacklo_##suffix(a0.val, a1.val));       \
        __m256i t1 = cast_from(_mm256_unpacklo_##suffix(a2.val, a3.val));       \
        __m256i t2 = cast_from(_mm256_unpackhi_##suffix(a0.val, a1.val));       \
        __m256i t3 = cast_from(_mm256_unpackhi_##suffix(a2.val, a3.val));       \
        b0.val = cast_to(_mm256_unpacklo_epi64(t0, t1));                        \
        b1.val = cast_to(_mm256_unpackhi_epi64(t0, t1));                        \
        b2.val = cast_to(_mm256_unpacklo_epi64(t2, t3));                        \
        b3.val = cast_to(_mm256_unpackhi_epi64(t2, t3));                        \
    }

SIMD_TRANSPOSE4x4(v_uint32x8,  epi32, SIMD_NOP, SIMD_NOP)
SIMD_TRANSPOSE4x4(v_int32x8,   epi32, SIMD_NOP, SIMD_NOP)
SIMD_TRANSPOSE4x4(v_float32x8, ps, _mm256_castps_si256, _mm256_castsi256_ps)

//////////////// Value reordering ///////////////

/* Expand */
#define SIMD_EXPAND(_Tpvec, _Tpwvec, _Tp, intrin)                   \
    inline void v_expand(const _Tpvec& a, _Tpwvec& b0, _Tpwvec& b1) \
    {                                                               \
        b0.val = intrin(_v256_extract_low(a.val));                  \
        b1.val = intrin(_v256_extract_high(a.val));                 \
    }                                                               \
    inline _Tpwvec v_expand_low(const _Tpvec& a)                    \
    { return _Tpwvec(intrin(_v256_extract_low(a.val))); }           \
    inline _Tpwvec v_expand_high(const _Tpvec& a)                   \
    { return _Tpwvec(intrin(_v256_extract_high(a.val))); }          \
    inline _Tpwvec v256_load_expand(const _Tp* ptr)                 \
    {                                                               \
        __m128i a = _mm_loadu_si128((const __m128i*)ptr);           \
        return _Tpwvec(intrin(a));                                  \
    }

SIMD_EXPAND(v_uint8x32,  v_uint16x16, uint8_t,  _mm256_cvtepu8_epi16)
SIMD_EXPAND(v_int8x32,   v_int16x16,  int8_t,   _mm256_cvtepi8_epi16)
SIMD_EXPAND(v_uint16x16, v_uint32x8,  uint16_t, _mm256_cvtepu16_epi32)
SIMD_EXPAND(v_int16x16,  v_int32x8,   int16_t,  _mm256_cvtepi16_epi32)
SIMD_EXPAND(v_uint32x8,  v_uint64x4,  uint32_t, _mm256_cvtepu32_epi64)
SIMD_EXPAND(v_int32x8,   v_int64x4,   int32_t,  _mm256_cvtepi32_epi64)

#define SIMD_EXPAND_Q(_Tpvec, _Tp, intrin)                          \
    inline _Tpvec v256_load_expand_q(const _Tp* ptr)                \
    {                                                               \
        __m128i a = _mm_loadl_epi64((const __m128i*)ptr);           \
        return _Tpvec(intrin(a));                                   \
    }

SIMD_EXPAND_Q(v_uint32x8, uint8_t, _mm256_cvtepu8_epi32)
SIMD_EXPAND_Q(v_int32x8,  int8_t,  _mm256_cvtepi8_epi32)

/* pack */
// 16
inline v_int8x32 v_pack(const v_int16x16& a, const v_int16x16& b)
{ return v_int8x32(_v256_shuffle_odd_64(_mm256_packs_epi16(a.val, b.val))); }

inline v_uint8x32 v_pack(const v_uint16x16& a, const v_uint16x16& b)
{
    __m256i t = _mm256_set1_epi16(255);
    __m256i a1 = _mm256_min_epu16(a.val, t);
    __m256i b1 = _mm256_min_epu16(b.val, t);
    return v_uint8x32(_v256_shuffle_odd_64(_mm256_packus_epi16(a1, b1)));
}

inline v_uint8x32 v_pack_u(const v_int16x16& a, const v_int16x16& b)
{
    return v_uint8x32(_v256_shuffle_odd_64(_mm256_packus_epi16(a.val, b.val)));
}

inline void v_pack_store(int8_t* ptr, const v_int16x16& a)
{ v_store_low(ptr, v_pack(a, a)); }

inline void v_pack_store(uint8_t* ptr, const v_uint16x16& a)
{
    const __m256i m = _mm256_set1_epi16(255);
    __m256i am = _mm256_min_epu16(a.val, m);
            am =  _v256_shuffle_odd_64(_mm256_packus_epi16(am, am));
    v_store_low(ptr, v_uint8x32(am));
}

inline void v_pack_u_store(uint8_t* ptr, const v_int16x16& a)
{ v_store_low(ptr, v_pack_u(a, a)); }

template<int32_t n> inline
v_uint8x32 v_rshr_pack(const v_uint16x16& a, const v_uint16x16& b)
{
    // we assume that n > 0, and so the shifted 16-bit values can be treated as signed numbers.
    v_uint16x16 delta = v256_setall_u16((int16_t)(1 << (n-1)));
    return v_pack_u(v_reinterpret_as_s16((a + delta) >> n),
                    v_reinterpret_as_s16((b + delta) >> n));
}

template<int32_t n> inline
void v_rshr_pack_store(uint8_t* ptr, const v_uint16x16& a)
{
    v_uint16x16 delta = v256_setall_u16((int16_t)(1 << (n-1)));
    v_pack_u_store(ptr, v_reinterpret_as_s16((a + delta) >> n));
}

template<int32_t n> inline
v_uint8x32 v_rshr_pack_u(const v_int16x16& a, const v_int16x16& b)
{
    v_int16x16 delta = v256_setall_s16((int16_t)(1 << (n-1)));
    return v_pack_u((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_u_store(uint8_t* ptr, const v_int16x16& a)
{
    v_int16x16 delta = v256_setall_s16((int16_t)(1 << (n-1)));
    v_pack_u_store(ptr, (a + delta) >> n);
}

template<int32_t n> inline
v_int8x32 v_rshr_pack(const v_int16x16& a, const v_int16x16& b)
{
    v_int16x16 delta = v256_setall_s16((int16_t)(1 << (n-1)));
    return v_pack((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_store(int8_t* ptr, const v_int16x16& a)
{
    v_int16x16 delta = v256_setall_s16((int16_t)(1 << (n-1)));
    v_pack_store(ptr, (a + delta) >> n);
}

// 32
inline v_int16x16 v_pack(const v_int32x8& a, const v_int32x8& b)
{ return v_int16x16(_v256_shuffle_odd_64(_mm256_packs_epi32(a.val, b.val))); }

inline v_uint16x16 v_pack(const v_uint32x8& a, const v_uint32x8& b)
{ return v_uint16x16(_v256_shuffle_odd_64(_v256_packs_epu32(a.val, b.val))); }

inline v_uint16x16 v_pack_u(const v_int32x8& a, const v_int32x8& b)
{ return v_uint16x16(_v256_shuffle_odd_64(_mm256_packus_epi32(a.val, b.val))); }

inline void v_pack_store(int16_t* ptr, const v_int32x8& a)
{ v_store_low(ptr, v_pack(a, a)); }

inline void v_pack_store(uint16_t* ptr, const v_uint32x8& a)
{
    const __m256i m = _mm256_set1_epi32(65535);
    __m256i am = _mm256_min_epu32(a.val, m);
            am = _v256_shuffle_odd_64(_mm256_packus_epi32(am, am));
    v_store_low(ptr, v_uint16x16(am));
}

inline void v_pack_u_store(uint16_t* ptr, const v_int32x8& a)
{ v_store_low(ptr, v_pack_u(a, a)); }


template<int32_t n> inline
v_uint16x16 v_rshr_pack(const v_uint32x8& a, const v_uint32x8& b)
{
    // we assume that n > 0, and so the shifted 32-bit values can be treated as signed numbers.
    v_uint32x8 delta = v256_setall_u32(1 << (n-1));
    return v_pack_u(v_reinterpret_as_s32((a + delta) >> n),
                    v_reinterpret_as_s32((b + delta) >> n));
}

template<int32_t n> inline
void v_rshr_pack_store(uint16_t* ptr, const v_uint32x8& a)
{
    v_uint32x8 delta = v256_setall_u32(1 << (n-1));
    v_pack_u_store(ptr, v_reinterpret_as_s32((a + delta) >> n));
}

template<int32_t n> inline
v_uint16x16 v_rshr_pack_u(const v_int32x8& a, const v_int32x8& b)
{
    v_int32x8 delta = v256_setall_s32(1 << (n-1));
    return v_pack_u((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_u_store(uint16_t* ptr, const v_int32x8& a)
{
    v_int32x8 delta = v256_setall_s32(1 << (n-1));
    v_pack_u_store(ptr, (a + delta) >> n);
}

template<int32_t n> inline
v_int16x16 v_rshr_pack(const v_int32x8& a, const v_int32x8& b)
{
    v_int32x8 delta = v256_setall_s32(1 << (n-1));
    return v_pack((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_store(int16_t* ptr, const v_int32x8& a)
{
    v_int32x8 delta = v256_setall_s32(1 << (n-1));
    v_pack_store(ptr, (a + delta) >> n);
}

// 64
// Non-saturating pack
inline v_uint32x8 v_pack(const v_uint64x4& a, const v_uint64x4& b)
{
    __m256i a0 = _mm256_shuffle_epi32(a.val, _MM_SHUFFLE(0, 0, 2, 0));
    __m256i b0 = _mm256_shuffle_epi32(b.val, _MM_SHUFFLE(0, 0, 2, 0));
    __m256i ab = _mm256_unpacklo_epi64(a0, b0); // a0, a1, b0, b1, a2, a3, b2, b3
    return v_uint32x8(_v256_shuffle_odd_64(ab));
}

inline v_int32x8 v_pack(const v_int64x4& a, const v_int64x4& b)
{ return v_reinterpret_as_s32(v_pack(v_reinterpret_as_u64(a), v_reinterpret_as_u64(b))); }

inline void v_pack_store(uint32_t* ptr, const v_uint64x4& a)
{
    __m256i a0 = _mm256_shuffle_epi32(a.val, _MM_SHUFFLE(0, 0, 2, 0));
    v_store_low(ptr, v_uint32x8(_v256_shuffle_odd_64(a0)));
}

inline void v_pack_store(int32_t* ptr, const v_int64x4& b)
{ v_pack_store((uint32_t*)ptr, v_reinterpret_as_u64(b)); }

template<int32_t n> inline
v_uint32x8 v_rshr_pack(const v_uint64x4& a, const v_uint64x4& b)
{
    v_uint64x4 delta = v256_setall_u64((uint64_t)1 << (n-1));
    return v_pack((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_store(uint32_t* ptr, const v_uint64x4& a)
{
    v_uint64x4 delta = v256_setall_u64((uint64_t)1 << (n-1));
    v_pack_store(ptr, (a + delta) >> n);
}

template<int32_t n> inline
v_int32x8 v_rshr_pack(const v_int64x4& a, const v_int64x4& b)
{
    v_int64x4 delta = v256_setall_s64((int64_t)1 << (n-1));
    return v_pack((a + delta) >> n, (b + delta) >> n);
}

template<int32_t n> inline
void v_rshr_pack_store(int32_t* ptr, const v_int64x4& a)
{
    v_int64x4 delta = v256_setall_s64((int64_t)1 << (n-1));
    v_pack_store(ptr, (a + delta) >> n);
}

// pack boolean
inline v_uint8x32 v_pack_b(const v_uint16x16& a, const v_uint16x16& b)
{
    __m256i ab = _mm256_packs_epi16(a.val, b.val);
    return v_uint8x32(_v256_shuffle_odd_64(ab));
}

inline v_uint8x32 v_pack_b(const v_uint32x8& a, const v_uint32x8& b,
                           const v_uint32x8& c, const v_uint32x8& d)
{
    __m256i ab = _mm256_packs_epi32(a.val, b.val);
    __m256i cd = _mm256_packs_epi32(c.val, d.val);

    __m256i abcd = _v256_shuffle_odd_64(_mm256_packs_epi16(ab, cd));
    return v_uint8x32(_mm256_shuffle_epi32(abcd, _MM_SHUFFLE(3, 1, 2, 0)));
}

inline v_uint8x32 v_pack_b(const v_uint64x4& a, const v_uint64x4& b, const v_uint64x4& c,
                           const v_uint64x4& d, const v_uint64x4& e, const v_uint64x4& f,
                           const v_uint64x4& g, const v_uint64x4& h)
{
    __m256i ab = _mm256_packs_epi32(a.val, b.val);
    __m256i cd = _mm256_packs_epi32(c.val, d.val);
    __m256i ef = _mm256_packs_epi32(e.val, f.val);
    __m256i gh = _mm256_packs_epi32(g.val, h.val);

    __m256i abcd = _mm256_packs_epi32(ab, cd);
    __m256i efgh = _mm256_packs_epi32(ef, gh);
    __m256i pkall = _v256_shuffle_odd_64(_mm256_packs_epi16(abcd, efgh));

    __m256i rev = _mm256_alignr_epi8(pkall, pkall, 8);
    return v_uint8x32(_mm256_unpacklo_epi16(pkall, rev));
}

/* Recombine */
// its up there with load and store operations

/* Extract */
#define SIMD_EXTRACT(_Tpvec)                                        \
    template<int32_t s>                                             \
    inline _Tpvec v_extract(const _Tpvec& a, const _Tpvec& b)       \
    { return v_rotate_right<s>(a, b); }

SIMD_EXTRACT(v_uint8x32)
SIMD_EXTRACT(v_int8x32)
SIMD_EXTRACT(v_uint16x16)
SIMD_EXTRACT(v_int16x16)
SIMD_EXTRACT(v_uint32x8)
SIMD_EXTRACT(v_int32x8)
SIMD_EXTRACT(v_uint64x4)
SIMD_EXTRACT(v_int64x4)
SIMD_EXTRACT(v_float32x8)
SIMD_EXTRACT(v_float64x4)

template<int32_t i>
inline uint8_t v_extract_n(v_uint8x32 a)
{
    return (uint8_t)_v256_extract_epi8<i>(a.val);
}

template<int32_t i>
inline int8_t v_extract_n(v_int8x32 a)
{
    return (int8_t)v_extract_n<i>(v_reinterpret_as_u8(a));
}

template<int32_t i>
inline uint16_t v_extract_n(v_uint16x16 a)
{
    return (uint16_t)_v256_extract_epi16<i>(a.val);
}

template<int32_t i>
inline int16_t v_extract_n(v_int16x16 a)
{
    return (int16_t)v_extract_n<i>(v_reinterpret_as_u16(a));
}

template<int32_t i>
inline uint32_t v_extract_n(v_uint32x8 a)
{
    return (uint32_t)_v256_extract_epi32<i>(a.val);
}

template<int32_t i>
inline int32_t v_extract_n(v_int32x8 a)
{
    return (int32_t)v_extract_n<i>(v_reinterpret_as_u32(a));
}

template<int32_t i>
inline uint64_t v_extract_n(v_uint64x4 a)
{
    return (uint64_t)_v256_extract_epi64<i>(a.val);
}

template<int32_t i>
inline int64_t v_extract_n(v_int64x4 v)
{
    return (int64_t)v_extract_n<i>(v_reinterpret_as_u64(v));
}

template<int32_t i>
inline float v_extract_n(v_float32x8 v)
{
    union { uint32_t iv; float fv; } d;
    d.iv = v_extract_n<i>(v_reinterpret_as_u32(v));
    return d.fv;
}

template<int32_t i>
inline double v_extract_n(v_float64x4 v)
{
    union { uint64_t iv; double dv; } d;
    d.iv = v_extract_n<i>(v_reinterpret_as_u64(v));
    return d.dv;
}

template<int32_t i>
inline v_uint32x8 v_broadcast_element(v_uint32x8 a)
{
    static const __m256i perm = _mm256_set1_epi32((int8_t)i);
    return v_uint32x8(_mm256_permutevar8x32_epi32(a.val, perm));
}

template<int32_t i>
inline v_int32x8 v_broadcast_element(const v_int32x8 &a)
{ return v_reinterpret_as_s32(v_broadcast_element<i>(v_reinterpret_as_u32(a))); }

template<int32_t i>
inline v_float32x8 v_broadcast_element(const v_float32x8 &a)
{ return v_reinterpret_as_f32(v_broadcast_element<i>(v_reinterpret_as_u32(a))); }


///////////////////// load deinterleave /////////////////////////////

inline void v_load_deinterleave( const uint8_t* ptr, v_uint8x32& a, v_uint8x32& b )
{
    __m256i ab0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i ab1 = _mm256_loadu_si256((const __m256i*)(ptr + 32));

    const __m256i sh = _mm256_setr_epi8(0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15,
                                        0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15);
    __m256i p0 = _mm256_shuffle_epi8(ab0, sh);
    __m256i p1 = _mm256_shuffle_epi8(ab1, sh);
    __m256i pl = _mm256_permute2x128_si256(p0, p1, 0 + 2*16);
    __m256i ph = _mm256_permute2x128_si256(p0, p1, 1 + 3*16);
    __m256i a0 = _mm256_unpacklo_epi64(pl, ph);
    __m256i b0 = _mm256_unpackhi_epi64(pl, ph);
    a = v_uint8x32(a0);
    b = v_uint8x32(b0);
}

inline void v_load_deinterleave( const uint16_t* ptr, v_uint16x16& a, v_uint16x16& b )
{
    __m256i ab0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i ab1 = _mm256_loadu_si256((const __m256i*)(ptr + 16));

    const __m256i sh = _mm256_setr_epi8(0, 1, 4, 5, 8, 9, 12, 13, 2, 3, 6, 7, 10, 11, 14, 15,
                                        0, 1, 4, 5, 8, 9, 12, 13, 2, 3, 6, 7, 10, 11, 14, 15);
    __m256i p0 = _mm256_shuffle_epi8(ab0, sh);
    __m256i p1 = _mm256_shuffle_epi8(ab1, sh);
    __m256i pl = _mm256_permute2x128_si256(p0, p1, 0 + 2*16);
    __m256i ph = _mm256_permute2x128_si256(p0, p1, 1 + 3*16);
    __m256i a0 = _mm256_unpacklo_epi64(pl, ph);
    __m256i b0 = _mm256_unpackhi_epi64(pl, ph);
    a = v_uint16x16(a0);
    b = v_uint16x16(b0);
}

inline void v_load_deinterleave( const uint32_t* ptr, v_uint32x8& a, v_uint32x8& b )
{
    __m256i ab0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i ab1 = _mm256_loadu_si256((const __m256i*)(ptr + 8));

    enum { sh = 0+2*4+1*16+3*64 };
    __m256i p0 = _mm256_shuffle_epi32(ab0, sh);
    __m256i p1 = _mm256_shuffle_epi32(ab1, sh);
    __m256i pl = _mm256_permute2x128_si256(p0, p1, 0 + 2*16);
    __m256i ph = _mm256_permute2x128_si256(p0, p1, 1 + 3*16);
    __m256i a0 = _mm256_unpacklo_epi64(pl, ph);
    __m256i b0 = _mm256_unpackhi_epi64(pl, ph);
    a = v_uint32x8(a0);
    b = v_uint32x8(b0);
}

inline void v_load_deinterleave( const uint64_t* ptr, v_uint64x4& a, v_uint64x4& b )
{
    __m256i ab0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i ab1 = _mm256_loadu_si256((const __m256i*)(ptr + 4));

    __m256i pl = _mm256_permute2x128_si256(ab0, ab1, 0 + 2*16);
    __m256i ph = _mm256_permute2x128_si256(ab0, ab1, 1 + 3*16);
    __m256i a0 = _mm256_unpacklo_epi64(pl, ph);
    __m256i b0 = _mm256_unpackhi_epi64(pl, ph);
    a = v_uint64x4(a0);
    b = v_uint64x4(b0);
}

inline void v_load_deinterleave( const uint8_t* ptr, v_uint8x32& a, v_uint8x32& b, v_uint8x32& c )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 32));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 64));

    __m256i s02_low = _mm256_permute2x128_si256(bgr0, bgr2, 0 + 2*16);
    __m256i s02_high = _mm256_permute2x128_si256(bgr0, bgr2, 1 + 3*16);

    const __m256i m0 = _mm256_setr_epi8(0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
                                        0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0);
    const __m256i m1 = _mm256_setr_epi8(0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
                                        -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1);

    __m256i b0 = _mm256_blendv_epi8(_mm256_blendv_epi8(s02_low, s02_high, m0), bgr1, m1);
    __m256i g0 = _mm256_blendv_epi8(_mm256_blendv_epi8(s02_high, s02_low, m1), bgr1, m0);
    __m256i r0 = _mm256_blendv_epi8(_mm256_blendv_epi8(bgr1, s02_low, m0), s02_high, m1);

    const __m256i
    sh_b = _mm256_setr_epi8(0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10, 13,
                            0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10, 13),
    sh_g = _mm256_setr_epi8(1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14,
                            1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14),
    sh_r = _mm256_setr_epi8(2, 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15,
                            2, 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15);
    b0 = _mm256_shuffle_epi8(b0, sh_b);
    g0 = _mm256_shuffle_epi8(g0, sh_g);
    r0 = _mm256_shuffle_epi8(r0, sh_r);

    a = v_uint8x32(b0);
    b = v_uint8x32(g0);
    c = v_uint8x32(r0);
}

inline void v_load_deinterleave( const uint16_t* ptr, v_uint16x16& a, v_uint16x16& b, v_uint16x16& c )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 16));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 32));

    __m256i s02_low = _mm256_permute2x128_si256(bgr0, bgr2, 0 + 2*16);
    __m256i s02_high = _mm256_permute2x128_si256(bgr0, bgr2, 1 + 3*16);

    const __m256i m0 = _mm256_setr_epi8(0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1,
                                        0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0);
    const __m256i m1 = _mm256_setr_epi8(0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0,
                                        -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0);
    __m256i b0 = _mm256_blendv_epi8(_mm256_blendv_epi8(s02_low, s02_high, m0), bgr1, m1);
    __m256i g0 = _mm256_blendv_epi8(_mm256_blendv_epi8(bgr1, s02_low, m0), s02_high, m1);
    __m256i r0 = _mm256_blendv_epi8(_mm256_blendv_epi8(s02_high, s02_low, m1), bgr1, m0);
    const __m256i sh_b = _mm256_setr_epi8(0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5, 10, 11,
                                          0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5, 10, 11);
    const __m256i sh_g = _mm256_setr_epi8(2, 3, 8, 9, 14, 15, 4, 5, 10, 11, 0, 1, 6, 7, 12, 13,
                                          2, 3, 8, 9, 14, 15, 4, 5, 10, 11, 0, 1, 6, 7, 12, 13);
    const __m256i sh_r = _mm256_setr_epi8(4, 5, 10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15,
                                          4, 5, 10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15);
    b0 = _mm256_shuffle_epi8(b0, sh_b);
    g0 = _mm256_shuffle_epi8(g0, sh_g);
    r0 = _mm256_shuffle_epi8(r0, sh_r);

    a = v_uint16x16(b0);
    b = v_uint16x16(g0);
    c = v_uint16x16(r0);
}

inline void v_load_deinterleave( const uint32_t* ptr, v_uint32x8& a, v_uint32x8& b, v_uint32x8& c )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 8));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 16));

    __m256i s02_low = _mm256_permute2x128_si256(bgr0, bgr2, 0 + 2*16);
    __m256i s02_high = _mm256_permute2x128_si256(bgr0, bgr2, 1 + 3*16);

    __m256i b0 = _mm256_blend_epi32(_mm256_blend_epi32(s02_low, s02_high, 0x24), bgr1, 0x92);
    __m256i g0 = _mm256_blend_epi32(_mm256_blend_epi32(s02_high, s02_low, 0x92), bgr1, 0x24);
    __m256i r0 = _mm256_blend_epi32(_mm256_blend_epi32(bgr1, s02_low, 0x24), s02_high, 0x92);

    b0 = _mm256_shuffle_epi32(b0, 0x6c);
    g0 = _mm256_shuffle_epi32(g0, 0xb1);
    r0 = _mm256_shuffle_epi32(r0, 0xc6);

    a = v_uint32x8(b0);
    b = v_uint32x8(g0);
    c = v_uint32x8(r0);
}

inline void v_load_deinterleave( const uint64_t* ptr, v_uint64x4& a, v_uint64x4& b, v_uint64x4& c )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 4));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 8));

    __m256i s01 = _mm256_blend_epi32(bgr0, bgr1, 0xf0);
    __m256i s12 = _mm256_blend_epi32(bgr1, bgr2, 0xf0);
    __m256i s20r = _mm256_permute4x64_epi64(_mm256_blend_epi32(bgr2, bgr0, 0xf0), 0x1b);
    __m256i b0 = _mm256_unpacklo_epi64(s01, s20r);
    __m256i g0 = _mm256_alignr_epi8(s12, s01, 8);
    __m256i r0 = _mm256_unpackhi_epi64(s20r, s12);

    a = v_uint64x4(b0);
    b = v_uint64x4(g0);
    c = v_uint64x4(r0);
}

inline void v_load_deinterleave( const uint8_t* ptr, v_uint8x32& a, v_uint8x32& b, v_uint8x32& c, v_uint8x32& d )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 32));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 64));
    __m256i bgr3 = _mm256_loadu_si256((const __m256i*)(ptr + 96));
    const __m256i sh = _mm256_setr_epi8(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
                                        0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15);

    __m256i p0 = _mm256_shuffle_epi8(bgr0, sh);
    __m256i p1 = _mm256_shuffle_epi8(bgr1, sh);
    __m256i p2 = _mm256_shuffle_epi8(bgr2, sh);
    __m256i p3 = _mm256_shuffle_epi8(bgr3, sh);

    __m256i p01l = _mm256_unpacklo_epi32(p0, p1);
    __m256i p01h = _mm256_unpackhi_epi32(p0, p1);
    __m256i p23l = _mm256_unpacklo_epi32(p2, p3);
    __m256i p23h = _mm256_unpackhi_epi32(p2, p3);

    __m256i pll = _mm256_permute2x128_si256(p01l, p23l, 0 + 2*16);
    __m256i plh = _mm256_permute2x128_si256(p01l, p23l, 1 + 3*16);
    __m256i phl = _mm256_permute2x128_si256(p01h, p23h, 0 + 2*16);
    __m256i phh = _mm256_permute2x128_si256(p01h, p23h, 1 + 3*16);

    __m256i b0 = _mm256_unpacklo_epi32(pll, plh);
    __m256i g0 = _mm256_unpackhi_epi32(pll, plh);
    __m256i r0 = _mm256_unpacklo_epi32(phl, phh);
    __m256i a0 = _mm256_unpackhi_epi32(phl, phh);

    a = v_uint8x32(b0);
    b = v_uint8x32(g0);
    c = v_uint8x32(r0);
    d = v_uint8x32(a0);
}

inline void v_load_deinterleave( const uint16_t* ptr, v_uint16x16& a, v_uint16x16& b, v_uint16x16& c, v_uint16x16& d )
{
    __m256i bgr0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgr1 = _mm256_loadu_si256((const __m256i*)(ptr + 16));
    __m256i bgr2 = _mm256_loadu_si256((const __m256i*)(ptr + 32));
    __m256i bgr3 = _mm256_loadu_si256((const __m256i*)(ptr + 48));
    const __m256i sh = _mm256_setr_epi8(0, 1, 8, 9, 2, 3, 10, 11, 4, 5, 12, 13, 6, 7, 14, 15,
                                        0, 1, 8, 9, 2, 3, 10, 11, 4, 5, 12, 13, 6, 7, 14, 15);
    __m256i p0 = _mm256_shuffle_epi8(bgr0, sh);
    __m256i p1 = _mm256_shuffle_epi8(bgr1, sh);
    __m256i p2 = _mm256_shuffle_epi8(bgr2, sh);
    __m256i p3 = _mm256_shuffle_epi8(bgr3, sh);

    __m256i p01l = _mm256_unpacklo_epi32(p0, p1);
    __m256i p01h = _mm256_unpackhi_epi32(p0, p1);
    __m256i p23l = _mm256_unpacklo_epi32(p2, p3);
    __m256i p23h = _mm256_unpackhi_epi32(p2, p3);

    __m256i pll = _mm256_permute2x128_si256(p01l, p23l, 0 + 2*16);
    __m256i plh = _mm256_permute2x128_si256(p01l, p23l, 1 + 3*16);
    __m256i phl = _mm256_permute2x128_si256(p01h, p23h, 0 + 2*16);
    __m256i phh = _mm256_permute2x128_si256(p01h, p23h, 1 + 3*16);

    __m256i b0 = _mm256_unpacklo_epi32(pll, plh);
    __m256i g0 = _mm256_unpackhi_epi32(pll, plh);
    __m256i r0 = _mm256_unpacklo_epi32(phl, phh);
    __m256i a0 = _mm256_unpackhi_epi32(phl, phh);

    a = v_uint16x16(b0);
    b = v_uint16x16(g0);
    c = v_uint16x16(r0);
    d = v_uint16x16(a0);
}

inline void v_load_deinterleave( const uint32_t* ptr, v_uint32x8& a, v_uint32x8& b, v_uint32x8& c, v_uint32x8& d )
{
    __m256i p0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i p1 = _mm256_loadu_si256((const __m256i*)(ptr + 8));
    __m256i p2 = _mm256_loadu_si256((const __m256i*)(ptr + 16));
    __m256i p3 = _mm256_loadu_si256((const __m256i*)(ptr + 24));

    __m256i p01l = _mm256_unpacklo_epi32(p0, p1);
    __m256i p01h = _mm256_unpackhi_epi32(p0, p1);
    __m256i p23l = _mm256_unpacklo_epi32(p2, p3);
    __m256i p23h = _mm256_unpackhi_epi32(p2, p3);

    __m256i pll = _mm256_permute2x128_si256(p01l, p23l, 0 + 2*16);
    __m256i plh = _mm256_permute2x128_si256(p01l, p23l, 1 + 3*16);
    __m256i phl = _mm256_permute2x128_si256(p01h, p23h, 0 + 2*16);
    __m256i phh = _mm256_permute2x128_si256(p01h, p23h, 1 + 3*16);

    __m256i b0 = _mm256_unpacklo_epi32(pll, plh);
    __m256i g0 = _mm256_unpackhi_epi32(pll, plh);
    __m256i r0 = _mm256_unpacklo_epi32(phl, phh);
    __m256i a0 = _mm256_unpackhi_epi32(phl, phh);

    a = v_uint32x8(b0);
    b = v_uint32x8(g0);
    c = v_uint32x8(r0);
    d = v_uint32x8(a0);
}

inline void v_load_deinterleave( const uint64_t* ptr, v_uint64x4& a, v_uint64x4& b, v_uint64x4& c, v_uint64x4& d )
{
    __m256i bgra0 = _mm256_loadu_si256((const __m256i*)ptr);
    __m256i bgra1 = _mm256_loadu_si256((const __m256i*)(ptr + 4));
    __m256i bgra2 = _mm256_loadu_si256((const __m256i*)(ptr + 8));
    __m256i bgra3 = _mm256_loadu_si256((const __m256i*)(ptr + 12));

    __m256i l02 = _mm256_permute2x128_si256(bgra0, bgra2, 0 + 2*16);
    __m256i h02 = _mm256_permute2x128_si256(bgra0, bgra2, 1 + 3*16);
    __m256i l13 = _mm256_permute2x128_si256(bgra1, bgra3, 0 + 2*16);
    __m256i h13 = _mm256_permute2x128_si256(bgra1, bgra3, 1 + 3*16);

    __m256i b0 = _mm256_unpacklo_epi64(l02, l13);
    __m256i g0 = _mm256_unpackhi_epi64(l02, l13);
    __m256i r0 = _mm256_unpacklo_epi64(h02, h13);
    __m256i a0 = _mm256_unpackhi_epi64(h02, h13);

    a = v_uint64x4(b0);
    b = v_uint64x4(g0);
    c = v_uint64x4(r0);
    d = v_uint64x4(a0);
}

///////////////////////////// store interleave /////////////////////////////////////

inline void v_store_interleave( uint8_t* ptr, const v_uint8x32& x, const v_uint8x32& y,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i xy_l = _mm256_unpacklo_epi8(x.val, y.val);
    __m256i xy_h = _mm256_unpackhi_epi8(x.val, y.val);

    __m256i xy0 = _mm256_permute2x128_si256(xy_l, xy_h, 0 + 2*16);
    __m256i xy1 = _mm256_permute2x128_si256(xy_l, xy_h, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, xy0);
        _mm256_stream_si256((__m256i*)(ptr + 32), xy1);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, xy0);
        _mm256_store_si256((__m256i*)(ptr + 32), xy1);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, xy0);
        _mm256_storeu_si256((__m256i*)(ptr + 32), xy1);
    }
}

inline void v_store_interleave( uint16_t* ptr, const v_uint16x16& x, const v_uint16x16& y,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i xy_l = _mm256_unpacklo_epi16(x.val, y.val);
    __m256i xy_h = _mm256_unpackhi_epi16(x.val, y.val);

    __m256i xy0 = _mm256_permute2x128_si256(xy_l, xy_h, 0 + 2*16);
    __m256i xy1 = _mm256_permute2x128_si256(xy_l, xy_h, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, xy0);
        _mm256_stream_si256((__m256i*)(ptr + 16), xy1);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, xy0);
        _mm256_store_si256((__m256i*)(ptr + 16), xy1);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, xy0);
        _mm256_storeu_si256((__m256i*)(ptr + 16), xy1);
    }
}

inline void v_store_interleave( uint32_t* ptr, const v_uint32x8& x, const v_uint32x8& y,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i xy_l = _mm256_unpacklo_epi32(x.val, y.val);
    __m256i xy_h = _mm256_unpackhi_epi32(x.val, y.val);

    __m256i xy0 = _mm256_permute2x128_si256(xy_l, xy_h, 0 + 2*16);
    __m256i xy1 = _mm256_permute2x128_si256(xy_l, xy_h, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, xy0);
        _mm256_stream_si256((__m256i*)(ptr + 8), xy1);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, xy0);
        _mm256_store_si256((__m256i*)(ptr + 8), xy1);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, xy0);
        _mm256_storeu_si256((__m256i*)(ptr + 8), xy1);
    }
}

inline void v_store_interleave( uint64_t* ptr, const v_uint64x4& x, const v_uint64x4& y,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i xy_l = _mm256_unpacklo_epi64(x.val, y.val);
    __m256i xy_h = _mm256_unpackhi_epi64(x.val, y.val);

    __m256i xy0 = _mm256_permute2x128_si256(xy_l, xy_h, 0 + 2*16);
    __m256i xy1 = _mm256_permute2x128_si256(xy_l, xy_h, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, xy0);
        _mm256_stream_si256((__m256i*)(ptr + 4), xy1);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, xy0);
        _mm256_store_si256((__m256i*)(ptr + 4), xy1);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, xy0);
        _mm256_storeu_si256((__m256i*)(ptr + 4), xy1);
    }
}

inline void v_store_interleave( uint8_t* ptr, const v_uint8x32& a, const v_uint8x32& b, const v_uint8x32& c,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    const __m256i sh_b = _mm256_setr_epi8(
            0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15, 10, 5,
            0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15, 10, 5);
    const __m256i sh_g = _mm256_setr_epi8(
            5, 0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15, 10,
            5, 0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15, 10);
    const __m256i sh_r = _mm256_setr_epi8(
            10, 5, 0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15,
            10, 5, 0, 11, 6, 1, 12, 7, 2, 13, 8, 3, 14, 9, 4, 15);

    __m256i b0 = _mm256_shuffle_epi8(a.val, sh_b);
    __m256i g0 = _mm256_shuffle_epi8(b.val, sh_g);
    __m256i r0 = _mm256_shuffle_epi8(c.val, sh_r);

    const __m256i m0 = _mm256_setr_epi8(0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
                                               0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0);
    const __m256i m1 = _mm256_setr_epi8(0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0,
                                               0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0);

    __m256i p0 = _mm256_blendv_epi8(_mm256_blendv_epi8(b0, g0, m0), r0, m1);
    __m256i p1 = _mm256_blendv_epi8(_mm256_blendv_epi8(g0, r0, m0), b0, m1);
    __m256i p2 = _mm256_blendv_epi8(_mm256_blendv_epi8(r0, b0, m0), g0, m1);

    __m256i bgr0 = _mm256_permute2x128_si256(p0, p1, 0 + 2*16);
    __m256i bgr1 = _mm256_permute2x128_si256(p2, p0, 0 + 3*16);
    __m256i bgr2 = _mm256_permute2x128_si256(p1, p2, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgr0);
        _mm256_stream_si256((__m256i*)(ptr + 32), bgr1);
        _mm256_stream_si256((__m256i*)(ptr + 64), bgr2);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgr0);
        _mm256_store_si256((__m256i*)(ptr + 32), bgr1);
        _mm256_store_si256((__m256i*)(ptr + 64), bgr2);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgr0);
        _mm256_storeu_si256((__m256i*)(ptr + 32), bgr1);
        _mm256_storeu_si256((__m256i*)(ptr + 64), bgr2);
    }
}

inline void v_store_interleave( uint16_t* ptr, const v_uint16x16& a, const v_uint16x16& b, const v_uint16x16& c,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    const __m256i sh_b = _mm256_setr_epi8(
         0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5, 10, 11,
         0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5, 10, 11);
    const __m256i sh_g = _mm256_setr_epi8(
         10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5,
         10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15, 4, 5);
    const __m256i sh_r = _mm256_setr_epi8(
         4, 5, 10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15,
         4, 5, 10, 11, 0, 1, 6, 7, 12, 13, 2, 3, 8, 9, 14, 15);

    __m256i b0 = _mm256_shuffle_epi8(a.val, sh_b);
    __m256i g0 = _mm256_shuffle_epi8(b.val, sh_g);
    __m256i r0 = _mm256_shuffle_epi8(c.val, sh_r);

    const __m256i m0 = _mm256_setr_epi8(0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1,
                                               0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0);
    const __m256i m1 = _mm256_setr_epi8(0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0,
                                               -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0);

    __m256i p0 = _mm256_blendv_epi8(_mm256_blendv_epi8(b0, g0, m0), r0, m1);
    __m256i p1 = _mm256_blendv_epi8(_mm256_blendv_epi8(g0, r0, m0), b0, m1);
    __m256i p2 = _mm256_blendv_epi8(_mm256_blendv_epi8(r0, b0, m0), g0, m1);

    __m256i bgr0 = _mm256_permute2x128_si256(p0, p2, 0 + 2*16);
    //__m256i bgr1 = p1;
    __m256i bgr2 = _mm256_permute2x128_si256(p0, p2, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgr0);
        _mm256_stream_si256((__m256i*)(ptr + 16), p1);
        _mm256_stream_si256((__m256i*)(ptr + 32), bgr2);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgr0);
        _mm256_store_si256((__m256i*)(ptr + 16), p1);
        _mm256_store_si256((__m256i*)(ptr + 32), bgr2);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgr0);
        _mm256_storeu_si256((__m256i*)(ptr + 16), p1);
        _mm256_storeu_si256((__m256i*)(ptr + 32), bgr2);
    }
}

inline void v_store_interleave( uint32_t* ptr, const v_uint32x8& a, const v_uint32x8& b, const v_uint32x8& c,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i b0 = _mm256_shuffle_epi32(a.val, 0x6c);
    __m256i g0 = _mm256_shuffle_epi32(b.val, 0xb1);
    __m256i r0 = _mm256_shuffle_epi32(c.val, 0xc6);

    __m256i p0 = _mm256_blend_epi32(_mm256_blend_epi32(b0, g0, 0x92), r0, 0x24);
    __m256i p1 = _mm256_blend_epi32(_mm256_blend_epi32(g0, r0, 0x92), b0, 0x24);
    __m256i p2 = _mm256_blend_epi32(_mm256_blend_epi32(r0, b0, 0x92), g0, 0x24);

    __m256i bgr0 = _mm256_permute2x128_si256(p0, p1, 0 + 2*16);
    //__m256i bgr1 = p2;
    __m256i bgr2 = _mm256_permute2x128_si256(p0, p1, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgr0);
        _mm256_stream_si256((__m256i*)(ptr + 8), p2);
        _mm256_stream_si256((__m256i*)(ptr + 16), bgr2);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgr0);
        _mm256_store_si256((__m256i*)(ptr + 8), p2);
        _mm256_store_si256((__m256i*)(ptr + 16), bgr2);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgr0);
        _mm256_storeu_si256((__m256i*)(ptr + 8), p2);
        _mm256_storeu_si256((__m256i*)(ptr + 16), bgr2);
    }
}

inline void v_store_interleave( uint64_t* ptr, const v_uint64x4& a, const v_uint64x4& b, const v_uint64x4& c,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i s01 = _mm256_unpacklo_epi64(a.val, b.val);
    __m256i s12 = _mm256_unpackhi_epi64(b.val, c.val);
    __m256i s20 = _mm256_blend_epi32(c.val, a.val, 0xcc);

    __m256i bgr0 = _mm256_permute2x128_si256(s01, s20, 0 + 2*16);
    __m256i bgr1 = _mm256_blend_epi32(s01, s12, 0x0f);
    __m256i bgr2 = _mm256_permute2x128_si256(s20, s12, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgr0);
        _mm256_stream_si256((__m256i*)(ptr + 4), bgr1);
        _mm256_stream_si256((__m256i*)(ptr + 8), bgr2);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgr0);
        _mm256_store_si256((__m256i*)(ptr + 4), bgr1);
        _mm256_store_si256((__m256i*)(ptr + 8), bgr2);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgr0);
        _mm256_storeu_si256((__m256i*)(ptr + 4), bgr1);
        _mm256_storeu_si256((__m256i*)(ptr + 8), bgr2);
    }
}

inline void v_store_interleave( uint8_t* ptr, const v_uint8x32& a, const v_uint8x32& b,
                                const v_uint8x32& c, const v_uint8x32& d,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i bg0 = _mm256_unpacklo_epi8(a.val, b.val);
    __m256i bg1 = _mm256_unpackhi_epi8(a.val, b.val);
    __m256i ra0 = _mm256_unpacklo_epi8(c.val, d.val);
    __m256i ra1 = _mm256_unpackhi_epi8(c.val, d.val);

    __m256i bgra0_ = _mm256_unpacklo_epi16(bg0, ra0);
    __m256i bgra1_ = _mm256_unpackhi_epi16(bg0, ra0);
    __m256i bgra2_ = _mm256_unpacklo_epi16(bg1, ra1);
    __m256i bgra3_ = _mm256_unpackhi_epi16(bg1, ra1);

    __m256i bgra0 = _mm256_permute2x128_si256(bgra0_, bgra1_, 0 + 2*16);
    __m256i bgra2 = _mm256_permute2x128_si256(bgra0_, bgra1_, 1 + 3*16);
    __m256i bgra1 = _mm256_permute2x128_si256(bgra2_, bgra3_, 0 + 2*16);
    __m256i bgra3 = _mm256_permute2x128_si256(bgra2_, bgra3_, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgra0);
        _mm256_stream_si256((__m256i*)(ptr + 32), bgra1);
        _mm256_stream_si256((__m256i*)(ptr + 64), bgra2);
        _mm256_stream_si256((__m256i*)(ptr + 96), bgra3);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgra0);
        _mm256_store_si256((__m256i*)(ptr + 32), bgra1);
        _mm256_store_si256((__m256i*)(ptr + 64), bgra2);
        _mm256_store_si256((__m256i*)(ptr + 96), bgra3);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgra0);
        _mm256_storeu_si256((__m256i*)(ptr + 32), bgra1);
        _mm256_storeu_si256((__m256i*)(ptr + 64), bgra2);
        _mm256_storeu_si256((__m256i*)(ptr + 96), bgra3);
    }
}

inline void v_store_interleave( uint16_t* ptr, const v_uint16x16& a, const v_uint16x16& b,
                                const v_uint16x16& c, const v_uint16x16& d,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i bg0 = _mm256_unpacklo_epi16(a.val, b.val);
    __m256i bg1 = _mm256_unpackhi_epi16(a.val, b.val);
    __m256i ra0 = _mm256_unpacklo_epi16(c.val, d.val);
    __m256i ra1 = _mm256_unpackhi_epi16(c.val, d.val);

    __m256i bgra0_ = _mm256_unpacklo_epi32(bg0, ra0);
    __m256i bgra1_ = _mm256_unpackhi_epi32(bg0, ra0);
    __m256i bgra2_ = _mm256_unpacklo_epi32(bg1, ra1);
    __m256i bgra3_ = _mm256_unpackhi_epi32(bg1, ra1);

    __m256i bgra0 = _mm256_permute2x128_si256(bgra0_, bgra1_, 0 + 2*16);
    __m256i bgra2 = _mm256_permute2x128_si256(bgra0_, bgra1_, 1 + 3*16);
    __m256i bgra1 = _mm256_permute2x128_si256(bgra2_, bgra3_, 0 + 2*16);
    __m256i bgra3 = _mm256_permute2x128_si256(bgra2_, bgra3_, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgra0);
        _mm256_stream_si256((__m256i*)(ptr + 16), bgra1);
        _mm256_stream_si256((__m256i*)(ptr + 32), bgra2);
        _mm256_stream_si256((__m256i*)(ptr + 48), bgra3);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgra0);
        _mm256_store_si256((__m256i*)(ptr + 16), bgra1);
        _mm256_store_si256((__m256i*)(ptr + 32), bgra2);
        _mm256_store_si256((__m256i*)(ptr + 48), bgra3);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgra0);
        _mm256_storeu_si256((__m256i*)(ptr + 16), bgra1);
        _mm256_storeu_si256((__m256i*)(ptr + 32), bgra2);
        _mm256_storeu_si256((__m256i*)(ptr + 48), bgra3);
    }
}

inline void v_store_interleave( uint32_t* ptr, const v_uint32x8& a, const v_uint32x8& b,
                                const v_uint32x8& c, const v_uint32x8& d,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i bg0 = _mm256_unpacklo_epi32(a.val, b.val);
    __m256i bg1 = _mm256_unpackhi_epi32(a.val, b.val);
    __m256i ra0 = _mm256_unpacklo_epi32(c.val, d.val);
    __m256i ra1 = _mm256_unpackhi_epi32(c.val, d.val);

    __m256i bgra0_ = _mm256_unpacklo_epi64(bg0, ra0);
    __m256i bgra1_ = _mm256_unpackhi_epi64(bg0, ra0);
    __m256i bgra2_ = _mm256_unpacklo_epi64(bg1, ra1);
    __m256i bgra3_ = _mm256_unpackhi_epi64(bg1, ra1);

    __m256i bgra0 = _mm256_permute2x128_si256(bgra0_, bgra1_, 0 + 2*16);
    __m256i bgra2 = _mm256_permute2x128_si256(bgra0_, bgra1_, 1 + 3*16);
    __m256i bgra1 = _mm256_permute2x128_si256(bgra2_, bgra3_, 0 + 2*16);
    __m256i bgra3 = _mm256_permute2x128_si256(bgra2_, bgra3_, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgra0);
        _mm256_stream_si256((__m256i*)(ptr + 8), bgra1);
        _mm256_stream_si256((__m256i*)(ptr + 16), bgra2);
        _mm256_stream_si256((__m256i*)(ptr + 24), bgra3);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgra0);
        _mm256_store_si256((__m256i*)(ptr + 8), bgra1);
        _mm256_store_si256((__m256i*)(ptr + 16), bgra2);
        _mm256_store_si256((__m256i*)(ptr + 24), bgra3);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgra0);
        _mm256_storeu_si256((__m256i*)(ptr + 8), bgra1);
        _mm256_storeu_si256((__m256i*)(ptr + 16), bgra2);
        _mm256_storeu_si256((__m256i*)(ptr + 24), bgra3);
    }
}

inline void v_store_interleave( uint64_t* ptr, const v_uint64x4& a, const v_uint64x4& b,
                                const v_uint64x4& c, const v_uint64x4& d,
                                StoreMode mode = StoreMode::ADDR_UNALIGNED )
{
    __m256i bg0 = _mm256_unpacklo_epi64(a.val, b.val);
    __m256i bg1 = _mm256_unpackhi_epi64(a.val, b.val);
    __m256i ra0 = _mm256_unpacklo_epi64(c.val, d.val);
    __m256i ra1 = _mm256_unpackhi_epi64(c.val, d.val);

    __m256i bgra0 = _mm256_permute2x128_si256(bg0, ra0, 0 + 2*16);
    __m256i bgra1 = _mm256_permute2x128_si256(bg1, ra1, 0 + 2*16);
    __m256i bgra2 = _mm256_permute2x128_si256(bg0, ra0, 1 + 3*16);
    __m256i bgra3 = _mm256_permute2x128_si256(bg1, ra1, 1 + 3*16);

    if (mode == StoreMode::ADDR_ALIGNED_NOCACHE)
    {
        _mm256_stream_si256((__m256i*)ptr, bgra0);
        _mm256_stream_si256((__m256i*)(ptr + 4), bgra1);
        _mm256_stream_si256((__m256i*)(ptr + 8), bgra2);
        _mm256_stream_si256((__m256i*)(ptr + 12), bgra3);
    }
    else if (mode == StoreMode::ADDR_ALIGNED)
    {
        _mm256_store_si256((__m256i*)ptr, bgra0);
        _mm256_store_si256((__m256i*)(ptr + 4), bgra1);
        _mm256_store_si256((__m256i*)(ptr + 8), bgra2);
        _mm256_store_si256((__m256i*)(ptr + 12), bgra3);
    }
    else
    {
        _mm256_storeu_si256((__m256i*)ptr, bgra0);
        _mm256_storeu_si256((__m256i*)(ptr + 4), bgra1);
        _mm256_storeu_si256((__m256i*)(ptr + 8), bgra2);
        _mm256_storeu_si256((__m256i*)(ptr + 12), bgra3);
    }
}

#define SIMD_LOADSTORE_INTERLEAVE(_Tpvec0, _Tp0, suffix0, _Tpvec1, _Tp1, suffix1) \
inline void v_load_deinterleave( const _Tp0* ptr, _Tpvec0& a0, _Tpvec0& b0 ) \
{ \
    _Tpvec1 a1, b1; \
    v_load_deinterleave((const _Tp1*)ptr, a1, b1); \
    a0 = v_reinterpret_as_##suffix0(a1); \
    b0 = v_reinterpret_as_##suffix0(b1); \
} \
inline void v_load_deinterleave( const _Tp0* ptr, _Tpvec0& a0, _Tpvec0& b0, _Tpvec0& c0 ) \
{ \
    _Tpvec1 a1, b1, c1; \
    v_load_deinterleave((const _Tp1*)ptr, a1, b1, c1); \
    a0 = v_reinterpret_as_##suffix0(a1); \
    b0 = v_reinterpret_as_##suffix0(b1); \
    c0 = v_reinterpret_as_##suffix0(c1); \
} \
inline void v_load_deinterleave( const _Tp0* ptr, _Tpvec0& a0, _Tpvec0& b0, _Tpvec0& c0, _Tpvec0& d0 ) \
{ \
    _Tpvec1 a1, b1, c1, d1; \
    v_load_deinterleave((const _Tp1*)ptr, a1, b1, c1, d1); \
    a0 = v_reinterpret_as_##suffix0(a1); \
    b0 = v_reinterpret_as_##suffix0(b1); \
    c0 = v_reinterpret_as_##suffix0(c1); \
    d0 = v_reinterpret_as_##suffix0(d1); \
} \
inline void v_store_interleave( _Tp0* ptr, const _Tpvec0& a0, const _Tpvec0& b0, \
                                StoreMode mode = StoreMode::ADDR_UNALIGNED ) \
{ \
    _Tpvec1 a1 = v_reinterpret_as_##suffix1(a0); \
    _Tpvec1 b1 = v_reinterpret_as_##suffix1(b0); \
    v_store_interleave((_Tp1*)ptr, a1, b1, mode);      \
} \
inline void v_store_interleave( _Tp0* ptr, const _Tpvec0& a0, const _Tpvec0& b0, const _Tpvec0& c0, \
                                StoreMode mode = StoreMode::ADDR_UNALIGNED ) \
{ \
    _Tpvec1 a1 = v_reinterpret_as_##suffix1(a0); \
    _Tpvec1 b1 = v_reinterpret_as_##suffix1(b0); \
    _Tpvec1 c1 = v_reinterpret_as_##suffix1(c0); \
    v_store_interleave((_Tp1*)ptr, a1, b1, c1, mode);  \
} \
inline void v_store_interleave( _Tp0* ptr, const _Tpvec0& a0, const _Tpvec0& b0, \
                                const _Tpvec0& c0, const _Tpvec0& d0, \
                                StoreMode mode = StoreMode::ADDR_UNALIGNED ) \
{ \
    _Tpvec1 a1 = v_reinterpret_as_##suffix1(a0); \
    _Tpvec1 b1 = v_reinterpret_as_##suffix1(b0); \
    _Tpvec1 c1 = v_reinterpret_as_##suffix1(c0); \
    _Tpvec1 d1 = v_reinterpret_as_##suffix1(d0); \
    v_store_interleave((_Tp1*)ptr, a1, b1, c1, d1, mode); \
}

SIMD_LOADSTORE_INTERLEAVE(v_int8x32,   int8_t,  s8,  v_uint8x32,  uint8_t,  u8)
SIMD_LOADSTORE_INTERLEAVE(v_int16x16,  int16_t, s16, v_uint16x16, uint16_t, u16)
SIMD_LOADSTORE_INTERLEAVE(v_int32x8,   int32_t, s32, v_uint32x8,  uint32_t, u32)
SIMD_LOADSTORE_INTERLEAVE(v_float32x8, float,   f32, v_uint32x8,  uint32_t, u32)
SIMD_LOADSTORE_INTERLEAVE(v_int64x4,   int64_t, s64, v_uint64x4,  uint64_t, u64)
SIMD_LOADSTORE_INTERLEAVE(v_float64x4, double,  f64, v_uint64x4,  uint64_t, u64)

// FP16
inline v_float32x8 v256_load_expand(const float16_t* ptr)
{
    if (GlobalVars::HAS_FP16)
    {
        return v_float32x8(_mm256_cvtph_ps(_mm_loadu_si128((const __m128i*)ptr)));
    }
    else
    {
        float DECL_ALIGNED(32) buf[8];
        for (int32_t i = 0; i < 8; i++)
            buf[i] = (float)ptr[i];
        return v256_load_aligned(buf);
    }
}

inline void v_pack_store(float16_t* ptr, const v_float32x8& a)
{
    if (GlobalVars::HAS_FP16)
    {
        __m128i ah = _mm256_cvtps_ph(a.val, 0);
        _mm_storeu_si128((__m128i*)ptr, ah);
    }
    else
    {
        float DECL_ALIGNED(32) buf[8];
        v_store_aligned(buf, a);
        for (int32_t i = 0; i < 8; i++)
            ptr[i] = float16_t(buf[i]);
    }
}
// end of FP16

inline void v256_cleanup() { _mm256_zeroall(); }

#undef SIMD_BIN_OP
#undef SIMD_BIN_FUNC
#undef SIMD_SHIFT_OP
#undef SIMD_LOGIC_OP
#undef SIMD_SELECT
#undef SIMD_CMP_OP_OV
#undef SIMD_CMP_OP_INT
#undef SIMD_OP_64BIT
#undef SIMD_CMP_FLT
#undef SIMD_CMP_OP_FLT
#undef SIMD_ROTATE_CAST
#undef SIMD_ROTATE
#undef SIMD_REDUCE_32
#undef SIMD_REDUCE_16
#undef SIMD_REDUCE_8
#undef SIMD_REDUCE_FLT
#undef SIMD_CHECK
#undef SIMD_CHECK_SHORT
#undef SIMD_MULADD
#undef SIMD_MISC
#undef SIMD_ABS
#undef SIMD_SPLAT2_PS
#undef SIMD_TRANSPOSE4x4
#undef SIMD_EXPAND
#undef SIMD_EXPAND_Q
#undef SIMD_EXTRACT

typedef v_uint8x32   v_uint8;
typedef v_int8x32    v_int8;
typedef v_uint16x16  v_uint16;
typedef v_int16x16   v_int16;
typedef v_uint32x8   v_uint32;
typedef v_int32x8    v_int32;
typedef v_uint64x4   v_uint64;
typedef v_int64x4    v_int64;
typedef v_float32x8  v_float32;
typedef v_float64x4  v_float64;

SIMD_SCOPE_END
} // namespace SimdOpt
