# Game Data System Technical Documentation

## Overview

The game data system is a critical component of Linuxtrack that enables enhanced TrackIR communication between Windows games running under Wine and the Linux tracking system. This document provides a comprehensive technical overview of how the system works.

## Architecture

### File Flow
```
sgl.dat (encrypted) → game_data.c → gamedata.txt → Wine Bridge → Games
```

### Key Components
1. **Data Extraction**: `src/game_data.c` - Decrypts and parses `sgl.dat`
2. **Data Storage**: `gamedata.txt` - Human-readable game database
3. **Data Usage**: Wine bridge components - Lookup and apply game data
4. **Communication**: Enhanced TrackIR protocol with encryption

## Data Extraction Process

### Source: sgl.dat
- **Location**: Extracted from TrackIR installer or installation
- **Format**: RC4-encrypted XML containing game information
- **Encryption Key**: First 5 bytes of MD5 hash of "NaturalPoint"

### Processing: game_data.c
```c
bool get_game_data(const char *input_fname, const char *output_fname, bool from_update)
{
    // 1. Decrypt sgl.dat using RC4
    // 2. Parse XML structure
    // 3. Extract Game elements with ApplicationID
    // 4. Write to gamedata.txt
}
```

### Output: gamedata.txt
- **Format**: Simple text file with game entries
- **Location**: `~/.config/linuxtrack/tir_firmware/gamedata.txt`
- **Structure**: `[GameID] "[GameName]" ([ApplicationID])`

## Data Structure

### game_desc_t Structure
```c
typedef struct {
    const char *name;      // Game name (e.g., "Battlefield 2")
    bool encrypted;        // Whether enhanced interface is used
    uint32_t key1, key2;   // 64-bit encryption keys
} game_desc_t;
```

### File Format Examples
```
9601 "Battlefield 2" (B1560A2A73276592)    # Enhanced interface
9701 "Priston Tale"                        # Basic interface
9602 "Battlefield 2142" (1C739AD3817FD433) # Enhanced interface
```

## Wine Bridge Integration

### Core Functions

#### 1. game_data_get_desc()
**Location**: `src/wine_bridge/client/rest.c`
**Purpose**: Lookup game information by ID

```c
bool game_data_get_desc(int id, game_desc_t *gd)
{
    // 1. Open gamedata.txt
    // 2. Parse each line with sscanf()
    // 3. Match game ID
    // 4. Extract name and encryption keys
    // 5. Return game description
}
```

#### 2. NPCLIENT_NP_RegisterProgramProfileID()
**Location**: `src/wine_bridge/client/NPClient_main.c`
**Purpose**: Register game with TrackIR system

```c
int __stdcall NPCLIENT_NP_RegisterProgramProfileID(unsigned short id)
{
    game_desc_t gd;
    if(game_data_get_desc(id, &gd)) {
        // Set up enhanced communication
        crypted = gd.encrypted;
        if(gd.encrypted) {
            // Extract 8-byte encryption table from key1/key2
            table[0-3] = gd.key1 bytes
            table[4-7] = gd.key2 bytes
        }
        linuxtrack_init(gd.name);
    }
}
```

### Parsing Logic
The `game_data_get_desc()` function uses `sscanf()` to parse lines:

```c
// For enhanced games (with ApplicationID)
sscanf(line, "%d \"%[^\"]\" (%08x%08x)", &id, &name, &key1, &key2);

// For basic games (no ApplicationID)
sscanf(line, "%d \"%[^\"]\"", &id, &name);
```

## Enhanced Communication Protocol

### Encryption Process
When `crypted = true`, data transmission uses the encryption keys:

1. **Key Extraction**: Convert `key1` and `key2` to 8-byte table
2. **Data Encryption**: Apply `enhance()` function to tracking data
3. **Checksum**: Calculate and verify data integrity

### Data Flow
```
Game Request → NP_RegisterProgramProfileID(id) → Lookup gamedata.txt
                                                      ↓
Game Data ← NP_GetData() ← Enhanced Data ← Apply Encryption ← Raw Tracking Data
```

## File Locations and Dependencies

### Primary Files
- `src/game_data.c` - Data extraction and parsing
- `src/wine_bridge/client/rest.c` - Game data lookup
- `src/wine_bridge/client/NPClient_main.c` - Game registration
- `src/wine_bridge/client/rest.h` - Data structure definitions

### Configuration Files
- `~/.config/linuxtrack/tir_firmware/gamedata.txt` - Game database
- `~/.config/linuxtrack/tir_firmware/sgl.dat` - Encrypted source data

### Debug Tools
- `debug_sgl.c` - Decrypt and examine sgl.dat
- `debug_xml_structure.c` - Analyze XML parsing
- `decrypted_sgl.xml` - Raw decrypted XML output

## Error Handling and Validation

### Common Issues
1. **Missing gamedata.txt**: Wine bridge can't find game information
2. **Corrupted ApplicationID**: Enhanced features unavailable
3. **Parse Errors**: Incorrect file format or structure
4. **Missing Encryption Keys**: Enhanced communication fails

### Validation Commands
```bash
# Check for ApplicationID entries
grep "(" ~/.config/linuxtrack/tir_firmware/gamedata.txt

# Verify specific game
grep "Battlefield 2" ~/.config/linuxtrack/tir_firmware/gamedata.txt

# Check for null entries (indicates parsing bug)
grep "((null))" ~/.config/linuxtrack/tir_firmware/gamedata.txt
```

## Performance Considerations

### File Access Patterns
- **Read-only**: gamedata.txt is only read, never written during runtime
- **Sequential Access**: Linear search through file for game ID
- **Caching**: No built-in caching - each lookup reads from disk

### Optimization Opportunities
1. **Indexed Lookup**: Could use hash table for faster game ID lookup
2. **Memory Caching**: Load entire file into memory for repeated lookups
3. **Binary Format**: Consider binary format for faster parsing

## Security Implications

### Encryption Keys
- **Source**: NaturalPoint's proprietary system
- **Usage**: Only for enhanced TrackIR communication
- **Storage**: Stored in plain text in gamedata.txt
- **Scope**: Limited to TrackIR protocol compatibility

### Data Integrity
- **Checksums**: Applied to encrypted data packets
- **Validation**: Game ID verification during registration
- **Fallback**: Basic interface if enhanced communication fails

## Troubleshooting

### Debugging Steps
1. **Verify File Exists**: Check `~/.config/linuxtrack/tir_firmware/gamedata.txt`
2. **Check File Format**: Ensure proper line format
3. **Test Game Lookup**: Use debug tools to verify parsing
4. **Monitor Wine Bridge**: Check for error messages during game registration

### Common Fixes
1. **Re-extract Data**: Run firmware extraction again
2. **Fix Parsing Bug**: Apply ApplicationID parsing fix
3. **Update Game Data**: Download latest sgl.dat from NaturalPoint
4. **Verify Permissions**: Ensure Wine can access gamedata.txt

## Future Enhancements

### Potential Improvements
1. **Database Backend**: Use SQLite for better performance
2. **Online Updates**: Automatic game data updates
3. **Enhanced Parsing**: More robust XML parsing
4. **Game Detection**: Automatic game ID detection
5. **Profile Management**: User-defined game profiles

### Compatibility
- **Backward Compatibility**: Maintain existing file format
- **Forward Compatibility**: Support for new game entries
- **Cross-Platform**: Ensure Wine bridge compatibility

## Related Documentation

- [ApplicationID Parsing Issue](../troubleshooting/APPLICATIONID_PARSING_ISSUE.md)
- [Firmware Extraction Troubleshooting](../troubleshooting/FIRMWARE_EXTRACTION_TROUBLESHOOTING.md)
- [Wine Support Documentation](../WINE_SUPPORT_MODERN.md)
- [Technical Implementation Guide](TECHNICAL_IMPLEMENTATION_GUIDE.md)

## Version History

- **2025-07-13**: Initial technical documentation
- Documented complete game data system architecture
- Added troubleshooting and debugging information
- Included performance and security considerations 