## Windows Driver Security and Development

This directory will focus on Windows Drivers, feature those that are vulnerable. And provide samples, utilities, and resources for 
development as well as exploitation.



### Driver List

Priveleged memory read/write vulnerabilities and more. Some PoCs are available and included.

 **ASUS**
 
```    
    EIO64.sys MmMapIoSpace/MmUnmapIoSpace
    IOMap64.sys MmMapIoSpace/MmUnmapIoSpace

    ATSZIO64.sys ZwMapViewOfSection/ZwUnmapViewOfSection/MmGetPhysicalAddress

    Device Name: ""\\.\ATSZIO"
    Map Physical IOCTL: 0x8807200C
    Unmap Physical IOCTL: 0x88072010
```
   [PoC](https://github.com/LimiQS/AsusDriversPrivEscala/blob/master/PoC-fixed.cs)
   
---------------------------------------

**ATI**

```
    atillk64.sys MmMapIoSpace/MmUnmapIoSpace/MmBuildMdlForNonPagedPool/MmMapLockedPages

    Device Name: "\\.\atillk64"
    Map/Unmap IOCTLs: 0x9C402534, 0x9C402538, 0x9C402544, 0x9C402548
    MDL IOCTLs: 0x9C40254C, 0x9C402558, 0x9C402560, 0x9C402564
```
---------------------------------------

**Avast**

```
    aswVmm.sys SSDT Hooking

    Device Name: "\\.\aswVmm"
    Hook IOCTL: 0xA000E804
```   
  [PoC](https://github.com/tanduRE/AvastHV/)
   
---------------------------------------

**Biostar**
```
    BS_Flash64.sys MmMapIoSpace/MmUnmapIoSpace/MmMapLockedPages/ExAllocatePoolWithTag/ExFreePoolWithTag

    Device Name: "\\.\BS_Flash64"
    Map/Unmap IOCTL: 0x222000
    Allocate IOCTL: 0x22203C

    BS_I2c64.sys MmMapIoSpace/MmUnmapIoSpace
    BSMEMx64.sys MmMapIoSpace/MmUnmapIoSpace/MmGetPhysicalAddress
    BSMIXP64.sys MmMapIoSpace/MmUnmapIoSpace/MmGetPhysicalAddress
```
---------------------------------------

**Capcom**
```
    Capcom.sys MmGetSystemRoutineAddress

    Device Name: "\\.\Htsysm72FB"
    Execute IOCTL: 0xAA013044
```
---------------------------------------

**CPUID**
```
    cpuz141.sys MmMapIoSpace/MmUnmapIoSpace

    Device Name: "\\.\cpuz141"
    Read Register IOCTL: 0x9C402428
    Physical Read: 0x9C402420
    Physical Write: 0x9C402430

    Notes: CVE-2017-15303
```
---------------------------------------

**CrystalMark**
```
    WinRing0x64.sys MmMapIoSpace/MmUnmapIoSpace

    Device Name: "\\.\WinRing0_1_0_1"
    Map/Unmap IOCTL: 0x9C406104
```
---------------------------------------

**Huawei**

```
    HwOs2Ec10x64.sys MmMapIoSpaceEx/KeInitializeApc/KeInsertQueueApc

    Device Name: "\\.\HwOs2EcDevX64"
    Notes: CVE-2019-5241
```
_Extra Info_

`sub_140009160`

- allocates RWX page in some target process;
- resolves CreateProcessW and CloseHandle function pointers in the address space of the target process;
- copies a code area from the driver as well as what seemed to be a parameter block to the allocated page; and
- performs User APC injection targeting that page
- More Information [here](https://www.microsoft.com/security/blog/2019/03/25/from-alert-to-driver-vulnerability-microsoft-defender-atp-investigation-unearths-privilege-escalation-flaw/)
---------------------------------------

**LG Driver**
```
    lha.sys
    Map IOCTL: 0x9C402FD8
    Unmap IOCTL: 0x9C402FDC

    Device Name: "\\.\{E8F2FF20-6AF7-4914-9398-CE2132FE170F}"
    Notes: CVE-2019-8372
```    
   [PoC](http://jackson-t.ca/lg-driver-lpe.html)
   
---------------------------------------

MSI/Microstar
```
    NTIOLib_x64.sys MmMapIoSpace/MmUnmapIoSpace

    Device Name: "\\.\NTIOLib_LiveUpdate"
    Read IOCTL: 0xC3506104
    Write IOCTL: 0xC350A108

    Notes: Packaged with MSI
    Can read/write MSR
```
   [PoC](https://github.com/rwfpl/rewolf-msi-exploit)
   
---------------------------------------

PC-Doctor
```
    pcdsrvc_x64.pkms MmMapIoSpace/MmProbeAndLockPages/

    Device Name: "\\.\PCDSRVC{3B54B31B-D06B6431-06020200}_0"
    GetPhysicalAddress IOCTL: 0x222080
    Read/Write Physical IOCTL: 0x222084, 0x222088
    Read/Write MSR IOCTL: 0x222180/0x222184

    Notes: Packaged with Dell SupportAssist
```  
   [PoC](https://github.com/SamLarenN/SpeedFan-Exploit/blob/master/SpeedFan%20Exploit/Speedfan.cpp)
   
---------------------------------------

SOKNO S.R.L.
```
    speedfan.sys MmMapIoSpace/MmMapIoSpace/MmUnmapIoSpace

    Device Name: "\\.\SpeedFan"
    Read Physical IOCTL: 0x9C402428
    Write Physical IOCTL: 0x9C40242C
    Read MSR: 0x9C402438
```
   [PoC](https://github.com/SamLarenN/SpeedFan-Exploit/blob/master/SpeedFan%20Exploit/Speedfan.cpp)
   
---------------------------------------
**Zemana**
```
    zam64.sys ZwOpenProcess

    Device name: "\\.\ZemanaAntiMalware"
    Open full access handle IOCTL: 0x80002010/0x8000204C
    Notes: CVE-2018-6606
```    
   [PoC](https://github.com/SouhailHammou/Exploits/blob/master/CVE-2018-6606%20-%20MalwareFox%20AntiMalware%20LPE/Malwarefox_privescl_1.c)

---------------------------------------

## Vulnerable Drivers List

A download link will be provided shortly courtesy of [Namazso](https://www.unknowncheats.me/forum/members/911201.html)

```
    Name                   Signer                                      Description                                    SHA256                                                          
    ----                   ------                                      -----------                                    ------                                                          
    ADV64DRV.sys           "FUJITSU LIMITED "                                                                         04A85E359525D662338CAE86C1E59B1D7AA9BD12B920E8067503723DC1E03162
    Agent64.sys            "eSupport.com, Inc."                        DriverAgent Direct I/O for 64-bit Windows      05F052C64D192CF69A462A5EC16DDA0D43CA5D0245900C9FCB9201685A2E7748
    Agent64.sys            Phoenix Technologies Ltd                    DriverAgent Direct I/O for 64-bit Windows      4045AE77859B1DBF13972451972EAAF6F3C97BEA423E9E78F1C2F14330CD47CA
    Agent64.sys            Phoenix Technologies Ltd                    DriverAgent Direct I/O for 64-bit Windows      6948480954137987A0BE626C24CF594390960242CD75F094CD6AAA5C2E7A54FA
    Agent64.sys            "eSupport.com, Inc"                         DriverAgent Direct I/O for 64-bit Windows      8CB62C5D41148DE416014F80BD1FD033FD4D2BD504CB05B90EEB6992A382D58F
    Agent64.sys            "eSupport.com, Inc."                        DriverAgent Direct I/O for 64-bit Windows      B1D96233235A62DBB21B8DBE2D1AE333199669F67664B107BFF1AD49B41D9414
    ALSysIO64.sys          Artur Liberman                              ALSysIO                                        7196187FB1EF8D108B380D37B2AF8EFDEB3CA1F6EEFD37B5DC114C609147216D
    ALSysIO64.sys          Artur Liberman                              ALSysIO                                        7F375639A0DF7FE51E5518CF87C3F513C55BC117DB47D28DA8C615642EB18BFA
    amifldrv64.sys         "American Megatrends, Inc."                                                                42579A759F3F95F20A2C51D5AC2047A2662A2675B3FB9F46C1ED7F23393A0F00
    AsIO.sys               ASUSTeK Computer Inc.                                                                      2DA330A2088409EFC351118445A824F11EDBE51CF3D653B298053785097FE40E
    AsIO.sys               ASUSTeK Computer Inc.                                                                      436CCAB6F62FA2D29827916E054ADE7ACAE485B3DE1D3E5C6C62D3DEBF1480E7
    AsIO.sys               ASUSTeK Computer Inc.                                                                      B4D47EA790920A4531E3DF5A4B4B0721B7FEA6B49A35679F0652F1E590422602
    AsIO.sys               ASUSTeK Computer Inc.                                                                      DDE6F28B3F7F2ABBEE59D4864435108791631E9CB4CDFB1F178E5AA9859956D8
    AsrAutoChkUpdDrv.sys   ASROCK Incorporation                        AsrAutoChkUpdDrv Driver                        2AA1B08F47FBB1E2BD2E4A492F5D616968E703E1359A921F62B38B8E4662F0C4
    AsrDrv10.sys           ASROCK Incorporation                        ASRock IO Driver                               ECE0A900EA089E730741499614C0917432246CEB5E11599EE3A1BB679E24FD2C
    AsrDrv101.sys          ASROCK Incorporation                        ASRock IO Driver                               F40435488389B4FB3B945CA21A8325A51E1B5F80F045AB019748D0EC66056A8B
    AsrIbDrv.sys           ASROCK Incorporation                        RW-Everything Read & Write Driver              2A652DE6B680D5AD92376AD323021850DAB2C653ABF06EDF26120F7714B8E08A
    AsrOmgDrv.sys          ASROCK Incorporation                        ASRock IO Driver                               950A4C0C772021CEE26011A92194F0E58D61588F77F2873AA0599DFF52A160C9
    AsrRapidStartDrv.sys   ASROCK Incorporation                        RW-Everything Read & Write Driver              0AAFA9F47ACF69D46C9542985994FF5321F00842A28DF2396D4A3076776A83CB
    AsrSmartConnectDrv.sys ASROCK Incorporation                        RW-Everything Read & Write Driver              47F08F7D30D824A8F4BB8A98916401A37C0FD8502DB308ABA91FE3112B892DCC
    AsUpIO.sys             ASUSTeK Computer Inc.                                                                      B9A4E40A5D80FEDD1037EAED958F9F9EFED41EB01ADA73D51B5DCD86E27E0CBF
    atillk64.sys           "ATI Technologies, Inc"                     ATI Diagnostics Hardware Abstraction Sys       5C04C274A708C9A7D993E33BE3EA9E6119DC29527A767410DBAF93996F87369A
    BS_Def64.sys           ASUSTeK Computer Inc.                       Default BIOS Flash Driver                      0040153302B88BEE27EB4F1ECA6855039E1A057370F5E8C615724FA5215BADA3
    BS_Def64.sys           ASUSTeK Computer Inc.                       Default BIOS Flash Driver                      3326E2D32BBABD69FEB6024809AFC56C7E39241EBE70A53728C77E80995422A5
    BS_Def64.sys           ASUSTeK Computer Inc.                       Default BIOS Flash Driver                      36B9E31240AB0341873C7092B63E2E0F2CAB2962EBF9B25271C3A1216B7669EB
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      29E0062A017A93B2F2F5207A608A96DF4D554C5DE976BD0276C2590A03BD3E94
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      45ABDBCD4C0916B7D9FAAF1CD08543A3A5178871074628E0126A6EDA890D26E0
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      50DB5480D0392A7DD6AB5DF98389DC24D1ED1E9C98C9C35964B19DABCD6DC67F
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      607DC4C75AC7AEF82AE0616A453866B3B358C6CF5C8F9D29E4D37F844306B97C
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      61D6E40601FA368800980801A662A5B3B36E3C23296E8AE1C85726A56EF18CC8
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      74A846C61ADC53692D3040AFF4C1916F32987AD72B07FE226E9E7DBEFF1036C4
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      76FB4DEAEE57EF30E56C382C92ABFFE2CF616D08DBECB3368C8EE6B02E59F303
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      81939E5C12BD627FF268E9887D6FB57E95E6049F28921F3437898757E7F21469
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      9790A7B9D624B2B18768BB655DDA4A05A9929633CEF0B1521E79E40D7DE0A05B
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      9A1D66036B0868BBB1B2823209FEDEA61A301D5DD245F8E7D390BD31E52D663E
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      AA9AB1195DC866270E984F1BED5E1358D6EF24C515DFDB6C2A92D1E1B94BF608
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      AF095DE15A16255CA1B2C27DAD365DFF9AC32D2A75E8E288F5A1307680781685
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      D5586DC1E61796A9AE5E5D1CED397874753056C3DF2EB963A8916287E1929A71
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      D8459F7D707C635E2C04D6D6D47B63F73BA3F6629702C7A6E0DF0462F6478AE2
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      E81230217988F3E7EC6F89A06D231EC66039BDBA340FD8EBB2BBB586506E3293
    CITMDRV_AMD64.sys      IBM Polska Sp. z o.o.                                                                      F88EBB633406A086D9CCA6BC8B66A4EA940C5476529F9033A9E0463512A23A57
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      1C8DFA14888BB58848B4792FB1D8A921976A9463BE8334CFF45CC96F1276049A
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      22418016E980E0A4A2D01CA210A17059916A4208352C1018B0079CCB19AAF86A
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      405472A8F9400A54BB29D03B436CCD58CFD6442FE686F6D2ED4F63F002854659
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      49F75746EEBE14E5DB11706B3E58ACCC62D4034D2F1C05C681ECEF5D1AD933BA
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      4A3D4DB86F580B1680D6454BAEE1C1A139E2DDE7D55E972BA7C92EC3F555DCE2
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      4AB41816ABBF14D59E75B7FAD49E2CB1C1FEB27A3CB27402297A2A4793FF9DA7
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      54841D9F89E195196E65AA881834804FE3678F1CF6B328CAB8703EDD15E3EC57
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      5EE292B605CD3751A24E5949AAE615D472A3C72688632C3040DC311055B75A92
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      76B86543CE05540048F954FED37BDDA66360C4A3DDB8328213D5AEF7A960C184
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      7F190F6E5AB0EDAFD63391506C2360230AF4C2D56C45FC8996A168A1FC12D457
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      845F1E228DE249FC1DDF8DC28C39D03E8AD328A6277B6502D3932E83B879A65A
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      84BF1D0BCDF175CFE8AEA2973E0373015793D43907410AE97E2071B2C4B8E2D4
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      8EF0AD86500094E8FA3D9E7D53163AA6FEEF67C09575C169873C494ED66F057F
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      A56C2A2425EB3A4260CC7FC5C8D7BED7A3B4CD2AF256185F24471C668853AEE8
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      AC3F613D457FC4D44FA27B2E0B1BAA62C09415705EFB5A40A4756DA39B3AC165
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      B1334A71CC73B3D0C54F62D8011BEC330DFC355A239BF94A121F6E4C86A30A2E
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      B47BE212352D407D0EF7458A7161C66B47C2AEC8391DD101DF11E65728337A6A
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      B9B3878DDC5DFB237D38F8D25067267870AFD67D12A330397A8853209C4D889C
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      DB90E554AD249C2BD888282ECF7D8DA4D1538DD364129A3327B54F8242DD5653
    CITMDRV_IA64.sys       IBM Polska Sp. z o.o.                                                                      E61A54F6D3869B43C4ECEAC3016DF73DF67CCE03878C5A6167166601C5D3F028
    cpuz_x64.sys           CPUID                                       CPUID Driver                                   3871E16758A1778907667F78589359734F7F62F9DC953EC558946DCDBE6951E3
    GLCKIO2.sys            ASUSTeK Computer Inc.                                                                      3A5EC83FE670E5E23AEF3AFA0A7241053F5B6BE5E6CA01766D6B5F9177183C25
    GLCKIO2.sys            ASUSTeK Computer Inc.                                                                      61A1BDDDD3C512E681818DEBB5BEE94DB701768FC25E674FCAD46592A3259BD0
    HOSTNT.sys             "SafeNet, Inc."                             Hostnt 64-bit driver                           07B6D69BAFCFD767F1B63A490A8843C3BB1F8E1BBEA56176109B5743C8F7D357
    HwRwDrv.sys            Shuttle Inc.                                Hardware read & write driver                   21CCDD306B5183C00ECFD0475B3152E7D94B921E858E59B68A03E925D1715F21
    inpoutx64.sys          RISINTECH INC.                              Kernel level port access driver                2D83CCB1AD9839C9F5B3F10B1F856177DF1594C66CBBC7661677D4B462EBF44D
    inpoutx64.sys          RISINTECH INC.                              Kernel level port access driver                F581DECC2888EF27EE1EA85EA23BBB5FB2FE6A554266FF5A1476ACD1D29D53AF
    inpoutx64.sys          Red Fox UK Limited                          Kernel level port access driver                F8965FDCE668692C3785AFA3559159F9A18287BC0D53ABB21902895A8ECF221B
    iomem64.sys            "DT RESEARCH, INC. TAIWAN BRANCH"           DTR Kernel mode driver                         3D23BDBAF9905259D858DF5BF991EB23D2DC9F4ECDA7F9F77839691ACEF1B8C4
    iomem64.sys            "DT RESEARCH, INC. TAIWAN BRANCH"           DTR Kernel mode driver                         DD4A1253D47DE14EF83F1BC8B40816A86CCF90D1E624C5ADF9203AE9D51D4097
    msrhook.sys            ID TECH                                                                                    6DE84CAA2CA18673E01B91AF58220C60AECD5CCCF269725EC3C7F226B2167492
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        09BEDBF7A41E0F8DABE4F41D331DB58373CE15B2E9204540873A1884F38BDDE1
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        101402D4F5D1AE413DED499C78A5FCBBC7E3BAE9B000D64C1DD64E3C48C37558
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        131D5490CEB9A5B2324D8E927FEA5BECFC633015661DE2F4C2F2375A3A3B64C6
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        1DDFE4756F5DB9FB319D6C6DA9C41C588A729D9E7817190B027B38E9C076D219
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib_X64                                    1E8B0C1966E566A523D652E00F7727D8B0663F1DFDCE3B9A09B9ADFAEF48D8EE
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib For MSISimple_OC                       2BBE65CBEC3BB069E92233924F7EE1F95FFA16173FCEB932C34F68D862781250
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        MSI ComCenService Driver                       30706F110725199E338E9CC1C940D9A644D19A14F0EB8847712CBA4CACDA67AB
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib for MSIFrequency_CC                    3124B0411B8077605DB2A9B7909D8240E0D554496600E2706E531C93C931E1B5
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        38FA0C663C8689048726666F1C5E019FEAA9DA8278F1DF6FF62DA33961891D2A
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        39CFDE7D401EFCE4F550E0A9461F5FC4D71FA07235E1336E4F0B4882BD76550E
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib For NTIOLib_ECO                        3D9E83B189FCF5C3541C62D1F54A0DA0A4E5B62C3243D2989AFC46644056C8E3
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        3F2FDA9A7A9C57B7138687BBCE49A2E156D6095DDDABB3454EA09737E02C3FA5
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        47F0CDAA2359A63AD1389EF4A635F1F6EEE1F63BDF6EF177F114BDCDADC2E005
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        50D5EAA168C077CE5B7F15B3F2C43BD2B86B07B1E926C1B332F8CB13BD2E0793
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        56A3C9AC137D862A85B4004F043D46542A1B61C6ACB438098A9640469E2D80E7
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib For MSIRatio_CC                        591BD5E92DFA0117B3DAA29750E73E2DB25BAA717C31217539D30FFB1F7F3A52
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib_X64                                    5D530E111400785D183057113D70623E17AF32931668AB7C7FC826F0FD4F91A3
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        6F1FF29E2E710F6D064DC74E8E011331D807C32CC2A622CBE507FD4B4D43F8F4
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        79E2D37632C417138970B4FEBA91B7E10C2EA251C5EFE3D1FC6FA0190F176B57
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        85866E8C25D82C1EC91D7A8076C7D073CCCF421CF57D9C83D80D63943A4EDD94
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        89B0017BC30CC026E32B758C66A1AF88BD54C6A78E11EC2908FF854E00AC46BE
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib for MSIDDR_CC                          9254F012009D55F555418FF85F7D93B184AB7CB0E37AECDFDAB62CFE94DEA96B
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        952199C28332BC90CFD74530A77EE237967ED32B3C71322559C59F7A42187DC4
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        9529EFB1837B1005E5E8F477773752078E0A46500C748BC30C9B5084D04082E6
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        984A77E5424C6D099051441005F2938AE92B31B5AD8F6521C6B001932862ADD7
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib for DebugLED                           98B734DDA78C16EBCAA4AFEB31007926542B63B2F163B2F733FA0D00DBB344D8
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        99F4994A0E5BD1BF6E3F637D3225C69FF4CD620557E23637533E7F18D7D6CBA1
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        9C10E2EC4F9EF591415F9A784B93DC9C9CDAFA7C69602C0DC860C5B62222E449
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        A961F5939088238D76757669A9A81905E33F247C9C635B908DAAC146AE063499
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        A9706E320179993DADE519A83061477ACE195DAA1B788662825484813001F526
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        B7A20B5F15E1871B392782C46EBCC897929443D82073EE4DCB3874B6A5976B5D
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        CC586254E9E89E88334ADEE44E332166119307E79C2F18F6C2AB90CE8BA7FC9B
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        CD4A249C3EF65AF285D0F8F30A8A96E83688486AAB515836318A2559757A89BB
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib for MSIClock_CC                        CF4B5FA853CE809F1924DF3A3AE3C4E191878C4EA5248D8785DC7E51807A512B
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        D0BD1AE72AEB5F3EABF1531A635F990E5EAAE7FDD560342F915F723766C80889
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        D8B58F6A89A7618558E37AFC360CD772B6731E3BA367F8D58734ECEE2244A530
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        D92EAB70BCECE4432258C9C9A914483A2267F6AB5CE2630048D3A99E8CB1B482
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        E005E8D183E853A27AD3BB56F25489F369C11B0D47E3D4095AAD9291B3343BF1
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib For MSISimple_OC                       E68D453D333854787F8470C8BAEF3E0D082F26DF5AA19C0493898BCF3401E39A
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib for MSICPU_CC                          E83908EBA2501A00EF9E74E7D1C8B4FF1279F1CD6051707FD51824F87E4378FA
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        EF86C4E5EE1DBC4F81CD864E8CD2F4A2A85EE4475B9A9AB698A4AE1CC71FBEB0
    NTIOLib.sys            "MICRO-STAR INTERNATIONAL CO., LTD."        NTIOLib                                        F088B2BA27DACD5C28F8EE428F1350DCA4BC7C6606309C287C801B2E1DA1A53D
    NTIOLib.sys            Micro-Star Int'l Co. Ltd.                   NTIOLib                                        FD8669794C67B396C12FC5F08E9C004FDF851A82FAF302846878173E4FBECB03
    OpenLibSys.sys         Noriyuki MIYAZAKI                           OpenLibSys                                     91314768DA140999E682D2A290D48B78BB25A35525EA12C1B1F9634D14602B2C
    OpenLibSys.sys         Noriyuki MIYAZAKI                           OpenLibSys                                     F0605DDA1DEF240DC7E14EFA73927D6C6D89988C01EA8647B671667B2B167008
    Se64a.sys              EnTech Taiwan                               EnTech softEngine x64 kernel-mode driver       6CB51AE871FBD5D07C5AAD6FF8EEA43D34063089528603CA9CEB8B4F52F68DDC
    smep_capcom.sys        "CAPCOM Co.,Ltd."                                                                          DB2A9247177E8CDD50FE9433D066B86FFD2A84301AA6B2EB60F361CFFF077004
    smep_namco.sys         NAMCO BANDAI Online Inc.                                                                   7EC93F34EB323823EB199FBF8D06219086D517D0E8F4B9E348D7AFD41EC9FD5D
    SysInfo.sys            Noriyuki MIYAZAKI                                                                          7049F3C939EFE76A5556C2A2C04386DB51DAF61D56B679F4868BB0983C996EBB
    VProEventMonitor.sys   Symantec Corporation                        VProEventMonitor.Sys - Event Monitoring driver 7877C1B0E7429453B750218CA491C2825DAE684AD9616642EFF7B41715C70ACA
    WCPU.sys               ASUSTeK Computer Inc.                       ASUS TDE CPU Driver                            159E7C5A12157AF92E0D14A0D3EA116F91C09E21A9831486E6DC592C93C10980
    WINIODrv.sys           "Partner Tech(Shanghai)Co.,Ltd"                                                            3243AAB18E273A9B9C4280A57AECEF278E10BFFF19ABB260D7A7820E41739099
    WINIODrv.sys           "Partner Tech(Shanghai)Co.,Ltd"                                                            7CFA5E10DFF8A99A5D544B011F676BC383991274C693E21E3AF40CF6982ADB8C
    WINIODrv.sys           "Partner Tech(Shanghai)Co.,Ltd"                                                            C9B49B52B493B53CD49C12C3FA9553E57C5394555B64E32D1208F5B96A5B8C6E
    WinRing0.sys           "TOSHIBA AMERICA INFORMATION SYSTEMS, INC." WinRing0                                       3EC5AD51E6879464DFBCCB9F4ED76C6325056A42548D5994BA869DA9C4C039A8
    WinRing0.sys           Noriyuki MIYAZAKI                           WinRing0                                       47EAEBC920CCF99E09FC9924FEB6B19B8A28589F52783327067C9B09754B5E84
    WinRing0.sys           EVGA                                        WinRing0                                       A7B000ABBCC344444A9B00CFADE7AA22AB92CE0CADEC196C30EB1851AE4FA062
```
