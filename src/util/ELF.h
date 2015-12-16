#pragma once

#include <elf.h>

#include "Log.h"

class ELF {
public:
	const u8 *elf;
	const Elf32_Ehdr *header;
	bool valid;

	u32 sectionCount = 0;
	const Elf32_Shdr *sections = nullptr;

	const Elf32_Shdr *stringTableSection = nullptr;
	const char *stringTable = nullptr;

	const Elf32_Shdr *symbolTableSection = nullptr;
	const Elf32_Shdr *symbolTableStringSection = nullptr;
	const Elf32_Sym *symbolTable = nullptr;
	const char *symbolStringTable = nullptr;
	u32 symbolCount = 0;

	ELF(u8 *e) : elf(e), header(reinterpret_cast<Elf32_Ehdr*>(e)) {
		valid = verify();

		if (valid) {
			Logger::log("Valid ELF");
			Logger::logf("Number of sections: %d", header->e_shnum);

			sectionCount = header->e_shnum;
			sections = reinterpret_cast<const Elf32_Shdr*>(elf+header->e_shoff);
			stringTableSection = getSection(header->e_shstrndx);
			stringTable = (const char*)getSectionData(stringTableSection);

			Logger::logf("String Table Size: %p", stringTableSection->sh_size);

			symbolTableSection = findSection(SHT_SYMTAB);
			symbolTable = reinterpret_cast<const Elf32_Sym*>(getSectionData(symbolTableSection));
			symbolTableStringSection = getSection(symbolTableSection->sh_link);
			symbolStringTable = (const char*)getSectionData(symbolTableStringSection);
			symbolCount = symbolTableSection->sh_size/symbolTableSection->sh_entsize;

			Logger::logf("Number of symbols: %d", symbolCount);
		}
	}

	bool verify() {
		if (strncmp((const char*)header->e_ident, ELFMAG, 4)) {
			return false;
		}

		return header->e_machine == EM_PPC;
	}

	const Elf32_Shdr *getSection(u32 i) {
		return sections+i;
	}

	const Elf32_Shdr *findSection(u32 type) {
		u32 i;
		for (i=0; i<sectionCount; i++) {
			if (getSection(i)->sh_type == type) {
				return getSection(i);
			}
		}

		return nullptr;
	}

	const u8 *getSectionData(const Elf32_Shdr *section) {
		return elf+section->sh_offset;
	}

	const char *getString(u32 strid, const char *strtab = nullptr) {
		if (strtab == nullptr) strtab = stringTable;
		return strtab+strid;
	}

	const Elf32_Sym *getSymbol(u32 index) {
		return symbolTable+index;
	}

	const char *getSymbolName(const Elf32_Sym *sym) {
		return getString(sym->st_name, symbolStringTable);
	}

	const Elf32_Sym *findSymbol(const char *name) {
		u32 i;
		const Elf32_Sym *symbol = symbolTable;
		for (i=0; i<symbolCount; i++) {
			if (strcmp(name, getString(symbol->st_name, symbolStringTable)) == 0) {
				return symbol;
			}

			symbol++;
		}

		return nullptr;
	}
};
