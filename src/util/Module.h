#pragma once

#include <string>
#include <map>

#include <ogc/cache.h>
#include <malloc.h>

#include <util/ELF.h>
#include <util/SyncQueue.h>
#include <util/Thread.h>

class AllocSection {
public:
	std::string name;
	u8 *data = nullptr;
	u32 size;
	u32 flags;

	AllocSection(const char *name, const Elf32_Shdr *section, const u8 *rodata) :
		name(name) {
		size = section->sh_size;
		data = (u8*)memalign(32, size);
		flags = section->sh_flags;

		memcpy(data, rodata, size);

		if (flags & SHF_EXECINSTR) {
			ICInvalidateRange(data, size);
		}
	}
};

class Symbol {
public:
	std::string name;
	u32 address;
	u8 type = STT_OBJECT;
	const Elf32_Sym *elfSym = nullptr;

	Symbol(std::string n, void *a) : name(n), address(reinterpret_cast<u32>(a)) {

	}

	Symbol(std::string n, const Elf32_Sym *sym, AllocSection *section) : name(n), elfSym(sym) {
		address = sym->st_value;
		type = ELF32_ST_TYPE(sym->st_info);
	}
};

class Module {
public:
	static std::map<std::string, Symbol*> globalSymbolMap;
	static std::map<std::string, Module*> loadedModules;

	ELF *elf;
	std::map<std::string, Symbol*> symbols;
	std::map<const Elf32_Sym*, Symbol*> symbolMap;
	std::map<u32, AllocSection*> sections;

	Thread *thread = nullptr;
	std::map<std::string, void*> data;

	bool isReady = false;
	SyncQueue readyQueue;

	Module(ELF *e) : elf(e) {
		u32 i;

		for (i=1; i<elf->sectionCount; i++) {
			const Elf32_Shdr *section = e->getSection(i);
			const char *name = e->getString(section->sh_name);

			if (section->sh_flags & SHF_ALLOC) {
				sections[i] = new AllocSection(name, section, elf->getSectionData(section));
			}
		}

		for (i=1; i<elf->symbolCount; i++) {
			const Elf32_Sym *symbol = e->getSymbol(i);
			const char *name = e->getSymbolName(symbol);

			//Logger::logf("Symbol %s", name);

			Symbol *sym = symbolMap[symbol] = symbols[name] = new Symbol(name, symbol, sections[symbol->st_shndx]);

			if (symbol->st_shndx == STN_UNDEF) {
				if (globalSymbolMap.count(name) == 1) {
					//global with the same name
					sym->address = globalSymbolMap[name]->address;
				} else {
					//TODO: Cancel link when undefined symbols
					Logger::logf("Undefined symbol %s", name);
				}
			} else {
				AllocSection *section = sections.count(symbol->st_shndx) == 0 ? nullptr : sections[symbol->st_shndx];
				
				if (section) {
					sym->address = reinterpret_cast<u32>(section->data+sym->address);
				}
			}
		}
	}

	bool link();

	Symbol *getSymbol(std::string name) {
		if (symbols.count(name) == 1) {
			return symbols[name];
		}

		if (globalSymbolMap.count(name) == 1) {
			return globalSymbolMap[name];
		}

		return nullptr;
	}

	void ready() {
		isReady = true;
		readyQueue.broadcast();
	}

	void waitReady() {
		if (!isReady) {
			readyQueue.sleep();
		}
	}

	static void init();
	static Module *start(std::string name);

	static Module *get(std::string name) {
		return loadedModules.count(name) == 1 ? loadedModules[name] : nullptr;
	}

	//TODO: Module queues, a system where modules
	//can send and receive messages to other modules
	//and broadcast messages via a thread that
	//multiplexes message objects to other threads.
};
