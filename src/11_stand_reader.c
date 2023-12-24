/*
 * A program that reads a file containing the data of stands appearing in
 * "JoJo's Bizarre Adventure" and prints out some aggregate statistics.
 * Additionally, this data is also written to a markdown file formatted as a
 * table. The input file is in csv format.
 *
 * Concepts:
 * File I/O.
 * Using struct to organize data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 128
#define MAX_STAND_NAME_LEN 40
#define INITIAL_STAND_ARRAY_CAPACITY 10

const char CSV_DELIMITER[1] = ",";
const char INPUT_FILE_PATH[] = "./md_tst/stand_input.csv";
const char OUTPUT_FILE_PATH[] = "./md_tst/README.md";
const char TEMP_OUTPUT_PATH[] = "./md_tst/stand_output.md.temp";

struct JoJoStand {
  char name[MAX_STAND_NAME_LEN + 1]; /* One extra byte for the null terminator. */
  int power;
  int speed;
  int range;
  int persistence;
  int precision;
  int potential;
};

/* An unnamed struct to store aggregate statistics. */
struct {
  /* Used to calculate averages. */
  int total_power;
  int total_speed;
  int total_range;
  int total_persistence;
  int total_precision;
  int total_potential;

  /* Used to count the occurrences of each stat. */
  int count_e_or_below;
  int count_d;
  int count_c;
  int count_b;
  int count_a_or_above;
} stand_aggregate_stats;

void parse_line_to_stand(struct JoJoStand *stand, char *line);
int read_stat_text_to_int_and_count(char *stat);
void process_stand(struct JoJoStand *stand, FILE *output_handle);
void write_stat_to_table(int stat, FILE *output_handle);

int main(int argc, char const *argv[]) {
  printf("Reading stand data from <%s>...\n", INPUT_FILE_PATH);

  FILE *input_handle = fopen(INPUT_FILE_PATH, "r");
  if (input_handle == NULL) {
    printf("Failed to open the input file.\n");
    printf("Make sure to execute this in the project's root directory.\n");
    exit(1);
  } else {
    /* Skip the first header line. */
    char *header_line = malloc(MAX_LINE_LEN + 1);
    if (header_line == NULL) {
      printf("Header line allocation failure.\n");
      exit(1);
    }
    fgets(header_line, MAX_LINE_LEN + 1, input_handle);
    free(header_line);
  }

  /* Array of stands to hold all data with initial allocation. */
  int stands_capacity = INITIAL_STAND_ARRAY_CAPACITY;
  struct JoJoStand *stands = malloc(stands_capacity * sizeof(struct JoJoStand));
  if (stands == NULL) {
    printf("Stand array allocation failure.\n");
    exit(1);
  }

  int stands_count = 0;
  char *data_line = malloc(MAX_LINE_LEN + 1);
  if (data_line == NULL) {
    printf("Data line allocation failure.\n");
    exit(1);
  }

  /* Read the entire file and store stand data into the array. */
  while (fgets(data_line, MAX_LINE_LEN + 1, input_handle) != NULL) {
    /* Sanity check, skip empty line(s). */
    if (strlen(data_line) <= 2) {
      continue;
    }

    /* Remove linefeed. */
    data_line[strcspn(data_line, "\n")] = '\0';

    /* Allocate more memory if needed. */
    if (stands_count >= stands_capacity) {
      printf("Read %d rows.\n", stands_count);
      stands_capacity *= 2;
      stands = realloc(stands, stands_capacity * sizeof(struct JoJoStand));
      if (stands == NULL) {
        printf("Stand array reallocation failure.\n");
        exit(1);
      }
    }

    /* Parse each line into structured data. */
    parse_line_to_stand(stands + stands_count, data_line);
    ++stands_count;
  }

  /* Close resources after reading. */
  free(data_line);
  if (fclose(input_handle) != 0) {
    printf("Failed to close the input file, abort.\n");
    exit(1);
  }
  printf("Successfully loaded %d rows from the input file.\n\n", stands_count);

  printf("Processing all stand data...\n");

  /* Open a temporary file for writing. */
  FILE *output_handle = fopen(TEMP_OUTPUT_PATH, "w");
  if (output_handle == NULL) {
    printf("Failed to open the temp output file, abort.\n");
    exit(1);
  }

  /* Write markdown header. */
  fprintf(output_handle, "# JoJo's Stand Table\n\n");
  /* Write the table header. */
  fprintf(output_handle,
          "| Stand | Power | Speed | Range | Persistence | Precision | "
          "Potential | Total |\n");
  fprintf(output_handle, "| --- | --- | --- | --- | --- | --- | --- | --- |\n");

  /* Process each stand entity. */
  int i = 0;
  for (; i < stands_count; ++i) {
    process_stand(stands + i, output_handle);
  }

  /* Close resources after writing. */
  if (fclose(output_handle) != 0) {
    printf("Failed to close the temp output file, abort.\n");
    exit(1);
  }
  free(stands);

  /* Rename the temp file. */
  if (rename(TEMP_OUTPUT_PATH, OUTPUT_FILE_PATH) != 0) {
    printf("Unable to rename the temp output file.\n");
    printf("Please check and cleanup <%s>.\n", TEMP_OUTPUT_PATH);
    exit(1);
  }
  printf("Successfully processed %d stand entities.\n\n", stands_count);

  /* Print aggregate statistics. */
  printf("=== Aggregate Statistics ===\n");
  printf("Average power: %.2f\n", (double)stand_aggregate_stats.total_power / stands_count);
  printf("Average speed: %.2f\n", (double)stand_aggregate_stats.total_speed / stands_count);
  printf("Average range: %.2f\n", (double)stand_aggregate_stats.total_range / stands_count);
  printf("Average persistence: %.2f\n", (double)stand_aggregate_stats.total_persistence / stands_count);
  printf("Average precision: %.2f\n", (double)stand_aggregate_stats.total_precision / stands_count);
  printf("Average potential: %.2f\n", (double)stand_aggregate_stats.total_potential / stands_count);
  printf("Number of E or below: %d\n", stand_aggregate_stats.count_e_or_below);
  printf("Number of D: %d\n", stand_aggregate_stats.count_d);
  printf("Number of C: %d\n", stand_aggregate_stats.count_c);
  printf("Number of B: %d\n", stand_aggregate_stats.count_b);
  printf("Number of A or above: %d\n", stand_aggregate_stats.count_a_or_above);
  printf("=== End ===\n\n");

  printf("All done. Markdown file is created at %s.\n", OUTPUT_FILE_PATH);

  return 0;
}

int read_stat_text_to_int_and_count(char *stat) {
  int stat_score = 0;

  if (stat == NULL) {
    printf("Possibly missing stand stat in input data, abort.\n");
    exit(1);
  } else if (strcmp(stat, "None") == 0) {
    stand_aggregate_stats.count_e_or_below += 1;
  } else if (strcmp(stat, "E") == 0) {
    stand_aggregate_stats.count_e_or_below += 1;
    stat_score = 1;
  } else if (strcmp(stat, "D") == 0) {
    stand_aggregate_stats.count_d += 1;
    stat_score = 2;
  } else if (strcmp(stat, "C") == 0) {
    stand_aggregate_stats.count_c += 1;
    stat_score = 3;
  } else if (strcmp(stat, "B") == 0) {
    stand_aggregate_stats.count_b += 1;
    stat_score = 4;
  } else if (strcmp(stat, "A") == 0) {
    stand_aggregate_stats.count_a_or_above += 1;
    stat_score = 5;
  } else if (strcmp(stat, "Infi") == 0) {
    stand_aggregate_stats.count_a_or_above += 1;
    stat_score = 6;
  } else {
    printf("Unknown stand stat <%s>, abort.\n", stat);
    exit(1);
  }

  return stat_score;
}

void parse_line_to_stand(struct JoJoStand *stand, char *line) {
  char *data_token;

  /* Stand name. */
  if ((data_token = strtok(line, CSV_DELIMITER)) != NULL) {
    strncpy(stand->name, data_token, MAX_STAND_NAME_LEN + 1);
  } else {
    printf("Error parsing stand name from input data, abort.\n");
    exit(1);
  }

  /* Stand stats. */
  stand->power = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
  stand->speed = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
  stand->range = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
  stand->persistence = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
  stand->precision = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
  stand->potential = read_stat_text_to_int_and_count(strtok(NULL, CSV_DELIMITER));
}

void write_stat_to_table(int stat, FILE *output_handle) {
  if (stat == 0) {
    fprintf(output_handle, " %s |", "None");
  } else if (stat == 1) {
    fprintf(output_handle, " %s |", "E");
  } else if (stat == 2) {
    fprintf(output_handle, " %s |", "D");
  } else if (stat == 3) {
    fprintf(output_handle, " %s |", "C");
  } else if (stat == 4) {
    fprintf(output_handle, " %s |", "B");
  } else if (stat == 5) {
    fprintf(output_handle, " %s |", "A");
  } else if (stat == 6) {
    fprintf(output_handle, " %s |", "Infinity");
  }
}

void process_stand(struct JoJoStand *stand, FILE *output_handle) {
  fprintf(output_handle, "| %s |", stand->name);

  stand_aggregate_stats.total_power += stand->power;
  write_stat_to_table(stand->power, output_handle);
  stand_aggregate_stats.total_speed += stand->speed;
  write_stat_to_table(stand->speed, output_handle);
  stand_aggregate_stats.total_range += stand->range;
  write_stat_to_table(stand->range, output_handle);
  stand_aggregate_stats.total_persistence += stand->persistence;
  write_stat_to_table(stand->persistence, output_handle);
  stand_aggregate_stats.total_precision += stand->precision;
  write_stat_to_table(stand->precision, output_handle);
  stand_aggregate_stats.total_potential += stand->potential;
  write_stat_to_table(stand->potential, output_handle);

  fprintf(output_handle, " %d |\n",
          stand->power + stand->speed + stand->range + stand->persistence + stand->precision + stand->potential);
}
