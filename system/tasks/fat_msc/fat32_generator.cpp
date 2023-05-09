#include <stdint.h>
#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>

#include "tasks/data_log.h"

// Looks like two copies of the boot sector?? interesting
uint8_t bootsector[] = {
#include "fat32_bootsector.h"
		};

typedef struct __attribute__((packed)) {
	unsigned int days :5;    // 1-31
	unsigned int months :4;  // 1-12
	unsigned int years :7;   // Years since 1980
} FATdate_t;

typedef struct __attribute__((packed)) {
	unsigned int seconds_periods :5;  // 2 second resolution
	unsigned int minutes :6;          // 0-59
	unsigned int hours :5;            // 0-23
} FATtime_t;

enum class FileAttributes : uint8_t {
	READ_ONLY = 1,
	HIDDEN = 2,
	SYSTEM_FILE = 4,
	VOLUME_LABEL = 8,
	SUBDIRECTORY = 0x10,
	ARCHIVE = 0X20,
};

typedef struct __attribute__((packed)) {
	char filename[8];
	char extension[3];
	uint8_t attributes;

	// 10 bytes reserved, total
	uint8_t reserved[1];

	uint8_t creationTimeTenths;
	FATtime_t createTime;
	FATdate_t createDate;
	FATdate_t accessDate;

	// fat32-specific
	uint16_t startingClusterHighBytes;

	FATtime_t modifyTime;
	FATdate_t modifyDate;
	uint16_t startingClusterLowBytes;
	uint32_t fileSizeBytes;
} FileEntry_t;

struct FlightInFlash {
	// "pointer" to where the flight actually exists in onboard flash
	int flightNumber;
	uint32_t internalFlashStartAddress;
	uint32_t internalFlashOnePastEndAddress;

	uint32_t firstFlightCluster; // The first FAT cluster this flight "occupies". The
								 // FAT entry here should point to first + 1, and that
								 // to first + 2... up to the last. Note that the
								 // first file will occupy sector #2, technically
	uint32_t lastFlightCluster;  // The last FAT cluster this flight "occupies".
								 // FAT entry for this cluster should be the end
								 // of cluster marker.

	uint32_t flightDataFirstFatEntryIdx; // Index in the FAT of the first flight data
										 // entry. The flight has (lastFATcluster -
										 // firstFATcluster) + 1 many entries in the

	FileEntry_t actualDataFile;
};

#define MAX_FLIGHTS 100
FlightInFlash flightArray[MAX_FLIGHTS] = { 0 };
uint32_t maxFlightNum;

#define CLUSTER_SIZE_BYTES 0x1000

#define FAT_START_ADDRESS 0x4000

#define ROOT_DIRECTORY_ADDR 0x7e000
#define FIRST_FILE_ADDRESS 0x7f000
#define FIRST_FLIGHT_CLUSTER_NUM 3

/**
 * @param entry The entry to assign date/times to
 * @param gpsTimeStart Seconds since unix epoch of first packet of the flight
 * @param gpsTimeEnd Seconds since unix epoch of last packet of the flight
 */
void updateFileTimestamp(FileEntry_t *entry, int64_t gpsTimeStart,
		int64_t gpsTimeEnd) {
	// set created timetsamp to the first GPS time
	tm *dt = gmtime(&gpsTimeStart);
	if (dt && dt->tm_year > 20 && dt->tm_year < 300) {
		entry->createDate.years = dt->tm_year + 1900 - 1980;
		entry->createDate.months = dt->tm_mon;
		entry->createDate.days = dt->tm_mday;
		entry->createTime.hours = dt->tm_hour;
		entry->createTime.minutes = dt->tm_min;
		entry->createTime.seconds_periods = dt->tm_sec / 2;
		entry->creationTimeTenths = 0;
	}

	// Set modified timestamp to gps end time
	dt = gmtime(&gpsTimeEnd);
	if (dt && dt->tm_year > 20 && dt->tm_year < 300) {
		entry->modifyDate.years = dt->tm_year + 1900 - 1980;
		entry->modifyDate.months = dt->tm_mon;
		entry->modifyDate.days = dt->tm_mday;
		entry->modifyTime.hours = dt->tm_hour;
		entry->modifyTime.minutes = dt->tm_min;
		entry->modifyTime.seconds_periods = dt->tm_sec / 2;
	}
}

// Update the list of flights in the flightArray based on internal flash state
// This should be called before any of the other MSC functions start doing any
// work!
extern "C" {
void mapFlashToClusters() {
	// int bound = dataLog_getLastFlightNum() - 1;
	uint32_t bound = 3; // hard coded for now, lol

	printf("Generating for %lu flights!\n", bound);

	// The first flight will start at FAT cluster 3 (TODO Matt: Why 3?)
	int firstFreeCluster = 3;

	for (unsigned int i = 0; i < bound; i++) {
		const int datalog_flight_idx = i + 1;
		uint32_t firstSector, lastSector;
		dataLog_getFlightSectors(datalog_flight_idx, &firstSector, &lastSector);

		// Sector should be >0 always
		if (firstSector == 0 || lastSector == 0) {
			continue;
		}

		FlightInFlash &flight = flightArray[i];
		flight.flightNumber = i;
		flight.internalFlashStartAddress = firstSector * FLASH_MAX_SECTOR_BYTES;
		flight.internalFlashOnePastEndAddress = (lastSector + 1)
				* FLASH_MAX_SECTOR_BYTES + 1;

		// MVP -- fix to 1 sector LMAO
		flight.actualDataFile.fileSizeBytes = 512;
//		flight.actualDataFile.fileSizeBytes =
//				flight.internalFlashOnePastEndAddress
//						- flight.internalFlashStartAddress;

		dataLog_readFlightNumMetadata(datalog_flight_idx);
		FlightMetadata_s *meta = dataLog_getFlightMetadata();

		// Use the first GPS fix timestamp from metadata as the start time
		// This isn't actually correct -- we record the GPS timestamp when
		// we first get a fix, not the GPS timestamp of board start.
		// So the end time is actually later than it ought to be
		uint64_t startTime = meta->gpsTimestamp;  // seconds
		uint64_t endTime;

		// If not set, will be all 1's
		if (startTime < (uint64_t) -1) {
			uint64_t duration = dataLog_getLastFlightTimestamp(
					datalog_flight_idx) / 1000;
			endTime = startTime + duration;
			updateFileTimestamp(&flight.actualDataFile, startTime, endTime);
		}

		// Also set filename. We tell the compiler it's 9 chars long but then ignore the zero terminator
		char buff[9] = { 0 };
		snprintf(buff, sizeof(buff), "%03i_data", datalog_flight_idx);
		memcpy(flight.actualDataFile.filename, buff, 8);
		char hex_ext[3] = { 'h', 'e', 'x' };
		memcpy(flight.actualDataFile.extension, hex_ext, sizeof(hex_ext));

		// Figure out what clusters to allocate to which part of the flight
		flight.firstFlightCluster = firstFreeCluster;

		uint64_t flightSizeBytes = (lastSector - firstSector + 1)
				* FLASH_MAX_SECTOR_BYTES;
		// Need to round _up_ to get # of sectors in MSC land
		const uint64_t numClusters = (int) ceil(
				flightSizeBytes / CLUSTER_SIZE_BYTES);
		flight.lastFlightCluster = flight.firstFlightCluster + numClusters;

		flight.actualDataFile.startingClusterHighBytes =
				(flight.firstFlightCluster >> 16) & 0xffff;
		flight.actualDataFile.startingClusterLowBytes =
				flight.firstFlightCluster & 0xffff;

		printf("Data cluster %i %i\n",
				flight.actualDataFile.startingClusterHighBytes,
				flight.actualDataFile.startingClusterLowBytes);

		firstFreeCluster += numClusters;

//    flight.launched = meta->launched;
//    flight.flightDurationSeconds = duration;
	}
	maxFlightNum = bound;
}
}

// Get the flight # in flash from a given address in mass-storage-land
FlightInFlash* getFlightFromMSCaddress(uint32_t mscAddress) {
	uint32_t offsetFromDataStart = mscAddress - FIRST_FILE_ADDRESS;
	// FAT-cluster-numbers, so FIRST_FILE_ADDRESS actually occupies the third
	// cluster
	uint32_t clusterNumber = (offsetFromDataStart / CLUSTER_SIZE_BYTES)
			+ FIRST_FLIGHT_CLUSTER_NUM;

	for (int i = 0; i < MAX_FLIGHTS; i++) {
		FlightInFlash *it = flightArray + i;
		if (clusterNumber >= it->firstFlightCluster
				&& clusterNumber <= it->lastFlightCluster) {
			return it;
		}
	}

	return nullptr;
}

uint32_t clusterToMSCglobalAddress(uint32_t cluster) {
	return (cluster - FIRST_FLIGHT_CLUSTER_NUM) * CLUSTER_SIZE_BYTES +
	FIRST_FILE_ADDRESS;
}

/**
 * @brief Get the location of the first byte of a file, in MSC address space
 */
uint32_t getFileDataAddress(FileEntry_t file) {
	int cluster = (file.startingClusterHighBytes << 16)
			| file.startingClusterLowBytes;
	return clusterToMSCglobalAddress(cluster);
}

uint32_t mscAddressToClusterNumber(uint32_t mscAddress) {
	return (int) ceil(
			(mscAddress - FIRST_FILE_ADDRESS) / (int) CLUSTER_SIZE_BYTES) +
	FIRST_FLIGHT_CLUSTER_NUM;
}

/**
 * Find the corresponding data in physical flash for a given msc address,
 * and copy it into the cluster
 */
void copyFlighDataToCluster(uint32_t mscAddress, uint8_t *pCluster) {
	FlightInFlash *flight = getFlightFromMSCaddress(mscAddress);
	if (!flight)
		return;

	// The offset of the cluster, in bytes, relative to the start of the flight in
	// msc address space
	uint32_t mscClusterStartAddress_offsetFromStartOfFlight = mscAddress
			- clusterToMSCglobalAddress(flight->firstFlightCluster);

	(void) mscClusterStartAddress_offsetFromStartOfFlight;

	// Fill pCluster with flash data
//	dataLog_flashRead(
//			flight->internalFlashStartAddress
//					+ mscClusterStartAddress_offsetFromStartOfFlight, 512,
//			pCluster);
}

#define MIN(a, b) a < b ? a : b
#define FAT_ENTRY_BYTES 4  // fat32

extern "C" {

// This is the main function of the fake SD card. It handles creating the
// appropriate fake data based on cluster requested by host
// pCluster had better be >= 512 (one physical cluster)
void retrieveSector(uint32_t mscAddress, uint32_t lengthBytes,
		uint8_t *pSector) {
	memset(pSector, 0xff, 512);

	// First blob of data: the boot sector. Never changes
	if (mscAddress < sizeof(bootsector)) {
		// We can accidentally read past the end of the boot sector
		// so try not to do that
		uint8_t *pBootsectorEnd = bootsector + sizeof(bootsector);

		// TODO do I need a -1?
		memcpy(pSector, bootsector + mscAddress,
				(size_t) MIN(512, (unsigned long )(pBootsectorEnd - mscAddress)));
		return;
	}

	// Second blob of data: the FAT
	else if ((mscAddress >= FAT_START_ADDRESS)
			&& mscAddress < ROOT_DIRECTORY_ADDR) {
		// Generate the FAT. We can safely assume that all flights are actually
		// contiguous in MSC memory
		// first 2 entries are [f8 ff ff 0f] and [ff ff ff 0f]
		// if a cluster is the last of a file, write [ff ff ff 0f]
		// if not, write little-endian the next cluster -- ex, [05, 00, 00, 00]

		// EOC: end-of-chain marker
		static uint8_t end_of_chain[] = { 0xff, 0xff, 0xff, 0x0f };
		// Magic marker for the boot sector
		static uint8_t bootsector_fat_entry[] = { 0xf8, 0xff, 0xff, 0x0f };

		// write boot and root dir entries
		uint32_t offset = 0;
		if (mscAddress == FAT_START_ADDRESS) {
			printf("Adding fat at %lu\r\n", mscAddress);
			// start with boot and root dir clusters
			memcpy(pSector + offset, bootsector_fat_entry,
					sizeof(bootsector_fat_entry));
			offset += sizeof(bootsector_fat_entry);
			memcpy(pSector + offset, end_of_chain, sizeof(end_of_chain));
			offset += sizeof(end_of_chain);

			// MVP -- assume each flight has 1 sector, and that there are 3 flights
			memcpy(pSector + offset, end_of_chain, sizeof(end_of_chain));
			offset += sizeof(end_of_chain);
			memcpy(pSector + offset, end_of_chain, sizeof(end_of_chain));
			offset += sizeof(end_of_chain);
			memcpy(pSector + offset, end_of_chain, sizeof(end_of_chain));
			offset += sizeof(end_of_chain);
		}

		// MVP -- just return ASAP
		return;

		/*
		 TODO so I started a really hacky impl, but it turns out it's a bit more involved.
		 Flights in the fat aren't guaranteed to be aligned with the bounds of sectors, so
		 a flight could start on one sector and continue onto the next. We should probably
		 think more about the algorithm before just naively sending it, exp. coz the usb host
		 can just call this function in an entirely arbitrary order.

		 We should:
		 - Find the cluster # of the first entry in our FAT
		 - Find the Flight that should contain that cluster
		 - Do the rest of the logic I already have below
		 */

		int flightIdx = 0;
		while (offset < 512) {
			// Find the flight responsible for a given cluster

			// FAT32 -- each entry takes up 4 bytes
			auto clusterNum = offset / 4;

			FlightInFlash flight; // TODO

			// metadata will always come first
			if (clusterNum >= flight.firstFlightCluster) {
				// Check if it's time to write end-of-chain yet
				if (clusterNum < flight.lastFlightCluster) {
					uint32_t next_cluster = clusterNum + 1;
					uint8_t entry[4] = { (uint8_t) (next_cluster & 0xff),
							(uint8_t) ((next_cluster >> 8) & 0xff),
							(uint8_t) ((next_cluster >> 16) & 0xff),
							(uint8_t) ((next_cluster >> 24) & 0xff), };
					memcpy(pSector + offset, entry, sizeof(entry));
					offset += sizeof(entry);
				} else {
					memcpy(pSector + offset, end_of_chain,
							sizeof(end_of_chain));
					offset += sizeof(end_of_chain);
					flightIdx++;
				}
			} else {
				// Unknown, just write 0xff
				// memcpy(pSector + offset, eoc, sizeof(eoc));
				memset(pSector + offset, 0xff, 4);
				offset += sizeof(end_of_chain);
			}

		}

	}

	// Third blob of data: the root directory
	// Need to generate a 512-byte chunk of the root directory
	else if ((mscAddress >= ROOT_DIRECTORY_ADDR)
			&& mscAddress < FIRST_FILE_ADDRESS) {
		const bool add_root_dir = (mscAddress == ROOT_DIRECTORY_ADDR);
		if (add_root_dir) {
			printf("Adding root dir @ %lu\r\n", mscAddress);
			FileEntry_t entry = { .filename = { 'E', 'M', 'B', 'E', 'D', 'D',
					'E', 'D' }, .extension = { ' ', 'F', 'S', }, .attributes =
					(uint8_t) FileAttributes::VOLUME_LABEL, .reserved = { 0 },
					.creationTimeTenths = 0, .createTime = { }, .createDate =
							{ }, .accessDate = { }, .startingClusterHighBytes =
							0, .modifyTime = { .seconds_periods = 0, .minutes =
							59, .hours = 9, }, .modifyDate = { .days = 22,
							.months = 10, .years = 2022 - 1980 },
					.startingClusterLowBytes = 0, .fileSizeBytes = 0 };

			memcpy(pSector + 0, &entry, sizeof(entry));
		}

		// How far, in bytes, into the root directory we are
		const uint32_t offset = mscAddress - ROOT_DIRECTORY_ADDR;

		// We assume each dir entry takes up exactly 32 bits (no long file name
		// extension stupidity). Each entry is 32 bytes long, so 16 entries per cluster
		uint32_t firstEntryIdx = offset / sizeof(FileEntry_t);
		uint32_t lastEntryIdx = firstEntryIdx + (512 / sizeof(FileEntry_t));

		// Need to take out root dir entry we add above. This offsets everything
		// following by 1
		if (!add_root_dir) {
			firstEntryIdx -= 1;
		}
		lastEntryIdx -= 1;

		for (unsigned int i = firstEntryIdx; i < lastEntryIdx; i++) {
			size_t clusterOffset = i * sizeof(FileEntry_t) + 32;
			const uint32_t flight_num = i;
			if (flight_num < maxFlightNum) {
				FlightInFlash *it = &(flightArray[flight_num]);

				//printf("Adding flights %ui at offset %u\r\n", i, clusterOffset);
				memcpy(pSector + clusterOffset, &it->actualDataFile,
						sizeof(FileEntry_t));
			}
		}

		return;
	} else if (mscAddress >= FIRST_FILE_ADDRESS) {

		return;

		// Search through all flights
		for (unsigned int i = 0; i < maxFlightNum; i++) {
			FlightInFlash &flight = flightArray[i];

			uint64_t flightAddress = getFileDataAddress(flight.actualDataFile);

			auto mscClusterNum = mscAddressToClusterNumber(mscAddress);

			if ((mscClusterNum >= flight.firstFlightCluster)
					&& (mscClusterNum <= (flight.lastFlightCluster))) {
				// Within the flight data file! Though we aren't quite sure at what
				// offset because clusters are 0x1000 bytes, not 0x200 bytes, long.

				uint64_t offsetFromFlightStart = mscAddress - flightAddress;
				uint32_t bytesToRead = MIN(512,
						flight.internalFlashOnePastEndAddress
								- offsetFromFlightStart);
				hm_flashReadStart(0,
						flight.internalFlashStartAddress
								+ offsetFromFlightStart, bytesToRead, pSector);
			}
		}
	}
}
}
