#include "Module.h"

#include "elf.h"

#include "LinkDefs.h"

extern "C" void log(const char *s);

std::map<std::string, Symbol*> Module::globalSymbolMap;

void Module::init() {
	linkdef *ld = linkdefs;
	while (ld->name) {
		globalSymbolMap[ld->name] = new Symbol(ld->name, ld->address);
		ld++;
	}
}

bool Module::link() {
	//link stage 1: process relocations
	u32 i;

	for (i=0; i<elf->sectionCount; i++) {
		const Elf32_Shdr *section = elf->getSection(i);
		const char *name = elf->getString(section->sh_name);

		if (section->sh_type == SHT_REL) {
			Logger::log("TODO: SHT_REL");
			return false;
		} else if (section->sh_type == SHT_RELA) {
			u32 relocationCount = section->sh_size/section->sh_entsize;
			u32 ri;
			const Elf32_Rela *relocation = reinterpret_cast<const Elf32_Rela*>(elf->getSectionData(section));
			AllocSection *target = sections[section->sh_info];

			for (ri = 0; ri<relocationCount; ri++) {
				u32 type = ELF32_R_TYPE(relocation->r_info);
				const Elf32_Sym *symbol = elf->getSymbol(ELF32_R_SYM(relocation->r_info));
				const char *symbolName = elf->getSymbolName(symbol);
				//Logger::logf("%s %s %d", name, symbolName, relocation->r_addend);
				
				Symbol *sym = symbolMap[symbol];

				u32 address = 0;

				if (sym->type == STT_SECTION) {
					if (sym->elfSym) {
						//Logger::logf("%d %p", symbol->st_shndx, sections[sym->elfSym->st_shndx]->data);
						address = reinterpret_cast<u32>(sections[sym->elfSym->st_shndx]->data);
					}
				} else {
					address = sym->address;
				}

				u32 offset = reinterpret_cast<u32>(target->data+relocation->r_offset);

				//Logger::logf("Relocation Address: %p %p %d", address, offset, sym->type);
				u32 *data = reinterpret_cast<u32*>(target->data+relocation->r_offset);
				u16 *data16 = reinterpret_cast<u16*>(target->data+relocation->r_offset);
				u32 value = 0;
				u32 addend = relocation->r_addend;

				//Thanks https://github.com/Chadderz121/brainslug-wii
				switch (type) {
					case R_PPC_ADDR32:
					case R_PPC_ADDR24:
					case R_PPC_ADDR16:
					case R_PPC_ADDR16_HI:
					case R_PPC_ADDR16_HA:
					case R_PPC_ADDR16_LO:
					case R_PPC_ADDR14:
					case R_PPC_ADDR14_BRTAKEN:
					case R_PPC_ADDR14_BRNTAKEN:
					case R_PPC_UADDR32:
					case R_PPC_UADDR16: {
						value = address + addend;
						break;
					}
					case R_PPC_REL24:
					case R_PPC_REL14:
					case R_PPC_REL14_BRTAKEN:
					case R_PPC_REL14_BRNTAKEN:
					case R_PPC_REL32: {
						value = address + addend - offset;
						break;
					}
					case R_PPC_SECTOFF:
					case R_PPC_SECTOFF_LO:
					case R_PPC_SECTOFF_HI:
					case R_PPC_SECTOFF_HA: {
						value = offset + addend;
						break;
					}
					case R_PPC_EMB_NADDR32:
					case R_PPC_EMB_NADDR16:
					case R_PPC_EMB_NADDR16_LO:
					case R_PPC_EMB_NADDR16_HI:
					case R_PPC_EMB_NADDR16_HA: {
						value = addend - address;
						break;
					}
					default:
						Logger::logf("Unknown Relocation type %d", type);
						return false;
				}

				//TODO: Finish this up
				switch(type) {
					case R_PPC_NONE:
						break;
					case R_PPC_ADDR32:
					case R_PPC_UADDR32:
					case R_PPC_REL32:
					case R_PPC_SECTOFF:
					case R_PPC_EMB_NADDR32:
						*data = value;
						break;
					case R_PPC_REL24:
					case R_PPC_ADDR24:
						*data |= (value&0x03fffffc);
						break;
					case R_PPC_ADDR16:
						*data16 = value;
						break;
					case R_PPC_ADDR16_LO:
						*data16 = (value) & 0xFFFF;
						break;
					case R_PPC_ADDR16_HI:
						*data16 = (value & 0xFFFF)>>16;
						break;
					case R_PPC_ADDR16_HA:
						*data16 = (value >> 16) + ((value >> 15)&1);
						break;
					default:
						Logger::logf("Unknown Relocation type %d", type);
						return false;
				}

				relocation++;
			}
		}
	}
	//link stage 2: ???
	//link stage 3: profit
	//link stage 4: Hyaaaaah!

	return true;
}
