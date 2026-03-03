# SourceS3 (MU Online Season 3) — by Mayck (mgalhardocoder-cmyk)

Coleção de sources/ferramentas para **MU Online Season 3**, focada em:
- **Servidor/Emulador**
- **Assets/Mídias** (quando aplicável)
- **Utilitários** (tools de suporte)

Repositório: https://github.com/mkcoder-g/SourceS3

---

## Estrutura

- `01-EMULATOR/` — núcleo do servidor/emulador (lógica, sistemas e correções) :contentReference[oaicite:3]{index=3}  
- `02-MEDIA/` — mídias/recursos do projeto (assets, materiais de apoio) :contentReference[oaicite:4]{index=4}  
- `05-UTIL/` — utilitários e ferramentas auxiliares :contentReference[oaicite:5]{index=5}  
- `SEASON3.sln` — solution principal do projeto :contentReference[oaicite:6]{index=6}  

---

## O que tem aqui (resumo “de dono”)

Este projeto serve como base para **customizar e evoluir** um ambiente Season 3, com foco em estabilidade, sistemas e melhorias.

Pelo changelog, os pacotes mais recorrentes são:
- Ajustes/correções de estabilidade (crash, desconexões, compatibilidade) :contentReference[oaicite:7]{index=7}  
- Atualizações e integrações de **LUA** e sistemas relacionados :contentReference[oaicite:8]{index=8}  
- Melhorias em sistemas/eventos e configurações (ex.:​:contentReference[oaicite:9]{index=9}ntryLevel.dat`, etc.) :contentReference[oaicite:10]{index=10}  

> A ideia é :contentReference[oaicite:11]{index=11} pra ir plugando seus módulos e customizações sem reinventar tudo.

---

## Requisitos (Windows)

- Window:contentReference[oaicite:12]{index=12}l Studio (recomendado: **VS 2017/2019/2022** com toolset de C++)
- Windows SDK instalado

### Se o projeto tiver recursos MFC/RC (erro `afxres.h`)
Se aparecer **RC1015 cannot open include file 'afxres.h'** no `.rc`, quase sempre é:
- faltando o **C++ MFC/ATL** no Visual Studio, ou
- include path do Windows SDK/VC não configurado corretamente

Checklist rápido:
1. Visual Studio Installer → Modificar → **Desktop development with C++**
2. Marcar também:
   - **MFC/ATL** (C++ MFC for latest v143/v142)
   - **Windows 10/11 SDK**
3. Reabrir a solution e recompilar.

> Isso resolve o clássico “Main.rc não abre/compila” com `afxres.h`.

---

## Como compilar

1. Abra `SEASON3.sln` :contentReference[oaicite:13]{index=13}  
2. Selecione `Release` ou `Debug`
3. Selecione a plataforma (Win32/x64 conforme o projeto)
4. Build na solution ou no projeto que você quer

Dica: se vier muito erro de include/lib, vale conferir:
- **Toolset** do projeto (v141/v142/v143)
- Windows SDK selecionado nas propriedades
- Dependências (libs/dlls) se existirem no projeto

---

## Changelog / Atualizações

O histórico completo está no `CHANGELOG.txt`.  
Destaques dos últimos updates listados:
- Update 39 (2.0.4-1): fixes de crash e ajustes relacionados a LUA/plugin :contentReference[oaicite:14]{index=14}  
- Update 38 (2.0.4): correções de desconexão/compatibilidade + atualização LUA 5.3.1 :contentReference[oaicite:15]{index=15}  
- Update 37 (2.0.3): configs/eventos e melhorias de performance/client:contentReference[oaicite:16]{index=16}:contentReference[oaicite:17]{index=17}  

---

## License

MIT License. :contentReference[oaicite:18]{index=18}  
Copyright (c) 2026
