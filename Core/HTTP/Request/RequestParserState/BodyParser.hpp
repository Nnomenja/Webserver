#ifndef BODYPARSER_HPP
#define BODYPARSER_HPP

#include "./ARequestParserState.hpp"
#include <string>
#include <cstdlib>
#include <climits>
#include <cctype>

typedef enum e_chunk_parser_state
	{
		CHUNK_SIZE,
		CHUNK_DATA,
		CHUNK_DATA_END,
		CHUNK_FINAL_END
} t_chunk_parser_state;
class BodyParser: public ARequestParserState {
	private:
		size_t					_tmp;
		bool					_end;
		t_chunk_parser_state	_chunkState;
		std::string				_chunkSizeLine;
		long					_chunkBytesRemaining;
		bool					_chunkCR;

		void    readBodyThroughContentLength();	
		void	readBodyAsChuncked();
		long	parseChunkSize(const std::string &line) const;
		void	handleChunkSizeLineChar(char c);
		void	handleChunkDataDelimiterChar(char c);
		void	handleFinalChunkDelimiterChar(char c);
	public:
		BodyParser(Request *target, UnitConf_t endpoint);
		~BodyParser();

		void	execute();
};

#endif
