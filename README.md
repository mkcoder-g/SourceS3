# SourceS3 — MU Online Season 3

Repositório com base de **Season 3** para MU Online, organizado em módulos (emulador, mídias e utilitários) e uma solution principal para compilar no Visual Studio.

**Repo:** https://github.com/mkcoder-g/SourceS3

---

## Estrutura do projeto

- `01-EMULATOR/` — servidor/emulador (core e sistemas)
- `02-MEDIA/` — mídias/recursos auxiliares
- `05-UTIL/` — ferramentas e utilitários
- `SEASON3.sln` — solution principal

---

## O que tem (resumo)

Pelos updates do `CHANGELOG.txt`, o projeto reúne principalmente:

- Correções de estabilidade (crash/desconexões/compatibilidade)
- Integrações/atualizações relacionadas a **LUA**
- Ajustes de configs e sistemas/eventos (ex.: `SkillEnabler.txt`, `EventEntryLevel.dat`, melhorias gerais)

---

## Como compilar (Windows / Visual Studio)

1. Abra `SEASON3.sln`
2. Selecione `Release` ou `Debug`
3. Selecione a plataforma (Win32/x64 conforme existir)
4. Compile o projeto desejado

---

## Problema comum: RC1015 / afxres.h

Se aparecer o erro:

- `RC1015 cannot open include file 'afxres.h'`

Normalmente é falta do componente do Visual Studio:

1. Visual Studio Installer → **Modify**
2. Marque:
   - **Desktop development with C++**
   - **MFC/ATL**
   - **Windows 10/11 SDK**
3. Reabra a solution e compile novamente

---

## Changelog

Confira o arquivo `CHANGELOG.txt` para a lista completa de alterações.

### Destaques recentes
- Update 39 (2.0.4-1): fixes de crash e ajustes relacionados a LUA/plugin
- Update 38 (2.0.4): correções de desconexão/compatibilidade + atualização LUA 5.3.1
- Update 37 (2.0.3): configs/eventos e melhorias gerais (inclui performance)

---

## Licença

MIT
