#!/usr/bin/awk -f

BEGIN {
        file_i           = 1;
        last_file_i      = 0;
        uniq_file_n      = 0;

        max_cdline_cnt_l = 1;
        sum_cdline       = 0;
        max_filename_l   = 0;
}

function init_new_file(filename)
{
        cdline_cnt = 0;
        in_cblock  = 0;

        if (length(filename) >= max_filename_l) {
                max_filename_l = length(filename);
        }

        ######################################################################
        # SELECT: comment block
        # default: /* */ as code block
        ######################################################################
        hs_cblock_start = @/^\s*(\{-)(.*)$/
        hs_cblock_end   = @/^.*(-\})(.*)$/

        c_cblock_start  = @/^\s*(\/\*)(.*)$/
        c_cblock_end    = @/^.*(\*\/)(.*)$/

        py_cblock_start = @/^\s*(""")\s*$/
        py_cblock_end   = @/^\s*(""")\s*$/
        ######################################################################

        switch (filename) {
        #-------------------------------------------------------------#
        case /\.(py|pyi)$/:
                cblock_start = py_cblock_start;
                cblock_end   = py_cblock_end;
                break;
        #-------------------------------------------------------------#
        case /\.(hs|lhs)$/:
                cblock_start = hs_cblock_start;
                cblock_end   = hs_cblock_end;
                break;
        #-------------------------------------------------------------#
        case /\.(java|class)$/:
        case /\.(c|cpp|h|hpp|inl|)$/:
        default:
                cblock_start = c_cblock_start;
                cblock_end   = c_cblock_end;
                break;
        }

        ######################################################################
        # SELECT: single-line comment
        # default: # as the line comment prefix
        ######################################################################
        batch_cline   = @/^\s*(REM|::).*$/;
        percent_cline = @/^\s*(%).*$/;
        pound_cline   = @/^\s*(#).*$/;
        ml_cline      = @/^\s*(;).*$/;
        c_cline       = @/^\s*(\/\/).*$/
        dash_cline    = @/^\s*(--).*$/
        ######################################################################

        switch (ARGV[1]) {
        #-------------------------------------------------------------#
        case /\.(bat)$/:
                cline = batch_cline;
                break;
        #-------------------------------------------------------------#
        case /\.(ml|sml|scm|smt)$/:
                cline = ml_cline;
                break;
        #-------------------------------------------------------------#
        case /\.(java|class)$/:
        case /\.(c|cpp|h|hpp|inl|)$/:
                cline = c_cline;
                break;
        #-------------------------------------------------------------#
        case /\.(lua)$/:
        case /\.(hs|lhs)$/:
        case /\.(scpt|scptd|applescript)$/:
                cline = dash_cline;
                break;
        #-------------------------------------------------------------#
        case /\.(sql)$/:
        case /\.(tex)$/:
        case /\.(erl|herl)$/:
        case /\.(pl|pro)$/:
        case /\.(m|mat)$/:
        case /\.(ps)$/:
                cline = percent_cline;
                break;
        #-------------------------------------------------------------#
        default:
                cline = pound_cline
                break;
        }
}

function saveclose_file(file_i, cdline_cnt)
{
        line_map[ARGV[file_i]] = cdline_cnt;
        sum_cdline            += cdline_cnt;
        if (length(cdline_cnt) > max_cdline_cnt_l) {
                max_cdline_cnt_l = length(cdline_cnt);
        }
        if (length(sum_cdline) > max_cdline_cnt_l) {
                max_cdline_cnt_l = length(sum_cdline);
        }
}

function next_file()
{
        # save last file's result, if any
        if (last_file_i >= 1) {
                saveclose_file(last_file_i, cdline_cnt);
        }

        # starting at 2nd file, if duplicate found: skip
        if (last_file_i >= 1 && length(line_map[ARGV[file_i]]) > 0) {
                nextfile

        } else {
                uniq_file_n++;
        }

        init_new_file(ARGV[file_i]);
        file_i++;
        last_file_i = file_i - 1;
}
# If not empty line
length {
        if ( FNR == 1 ) {
                next_file();
        }

        if (in_cblock) {
                if ( $0 ~ cblock_end ) {
                        in_cblock = 0
                }
        }
        else {
                if ( $0 ~ cline ) {
                        # is a single-line comment
                } else if ( $0 ~ cblock_start ) {
                        in_cblock = 1
                } else {
                        cdline_cnt++;
                }
        }
}

function print_fileres(filename, cdline_cnt, use_unit) {
        printf("%s  %"max_filename_l"s%s: %s%"max_cdline_cnt_l"s%s\n",
                magenta_f, filename,
                cyan_f   ,
                reset_f  , cdline_cnt,
                use_unit ? dim_s" (lines)"reset_f : "");
}

END {
        # scrap last file's result
        if ( last_file_i >= 1 ) {
                saveclose_file(last_file_i, cdline_cnt)
        }

        magenta_f = "\033[35m";
        cyan_f    = "\033[36m";
        reset_f   = "\033[m";
        dim_s     = "\033[1m"

        for (file in line_map) {
                print_fileres(file, line_map[file], 0);
        }

        if ( uniq_file_n >= 2 ) {
                for (j = 0; j < (max_filename_l + max_cdline_cnt_l + 4); j++) {
                        printf("=")
                }
                print("")
                print_fileres("TOTAL", sum_cdline, 1);
        }
}