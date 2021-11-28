#pragma once

#define FLAG_TYPE(m_type) \
inline m_type operator~(m_type a) \
{ return static_cast<m_type>(~static_cast<uint32_t>(a)); } \
inline m_type operator|(m_type a, m_type b) \
{ return static_cast<m_type>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); } \
inline m_type operator&(m_type a, m_type b) \
{ return static_cast<m_type>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); }
