/*
 * nvbio
 * Copyright (c) 2011-2014, NVIDIA CORPORATION. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the NVIDIA CORPORATION nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

namespace nvbio {
namespace io {

// load from a memory mapped object
//
// \param name          memory mapped object name
inline
int SequenceDataMMAP::load(const char* name)
{
    log_visible(stderr, "SequenceData (MMAP) : loading... started\n");
    log_visible(stderr, "  file : %s\n", file_name);

    std::string infoName        = std::string("nvbio.") + std::string( file_name ) + ".seq_info";
    std::string seqName         = std::string("nvbio.") + std::string( file_name ) + ".seq";
    std::string seqIndexName    = std::string("nvbio.") + std::string( file_name ) + ".seq_index";
    std::string qualName        = std::string("nvbio.") + std::string( file_name ) + ".qual";
    std::string nameName        = std::string("nvbio.") + std::string( file_name ) + ".name";
    std::string nameIndexName   = std::string("nvbio.") + std::string( file_name ) + ".name_index";

    try {
        const Info* info = (const Info*)m_info_file.init( infoName.c_str(), sizeof(SequenceDataInfo) );

        this->SequenceDataInfo::operator=( *info );

        const uint64 index_file_size     = info->m_n_seqs * sizeof(uint32);
        const uint64 seq_file_size       = info->m_sequence_stream_words * sizeof(uint32);
        const uint64 qual_file_size = info->m_sequence_stream_len * sizeof(char);
        const uint64 name_file_size = info->m_name_stream_len * sizeof(char);

        m_sequence_ptr        = (uint32*)m_sequence_file.init( seqName.c_str(), seq_file_size );
        m_sequence_index_ptr  = (uint32*)m_sequence_index_file.init( seqIndexName.c_str(), index_file_size );
        m_qual_ptr            = (uint32*)m_qual_file.init( qualName.c_str(), qual_file_size );
        m_name_ptr            = (uint32*)m_qual_file.init( nameName.c_str(), name_file_size );
        m_name_index_ptr      = (uint32*)m_sequence_index_file.init( nameIndexName.c_str(), index_file_size );
    }
    catch (MappedFile::mapping_error error)
    {
        log_error(stderr, "SequenceDataMMAP: error mapping file \"%s\" (%d)!\n", error.m_file_name, error.m_code);
        return 0;
    }
    catch (MappedFile::view_error error)
    {
        log_error(stderr, "SequenceDataMMAP: error viewing file \"%s\" (%d)!\n", error.m_file_name, error.m_code);
        return 0;
    }
    catch (...)
    {
        log_error(stderr, "SequenceDataMMAP: error mapping file (unknown)!\n");
        return 0;
    }

    log_visible(stderr, "SequenceData (MMAP) : loading... done\n");
}

} // namespace io
} // namespace nvbio
