#!/usr/bin/awk -f

function init_new_file(filename)
{
        cdline_cnt=0;
        in_cblock=0;

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

function next_file()
{
        # save last file's result, if any
        if (last_file_i >= 1) {
                line_map[ARGV[last_file_i]] = cdline_cnt;
                if (length(cdline_cnt) > max_cdline_cnt_l) {
                        max_cdline_cnt_l = length(cdline_cnt);
                }
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

BEGIN {
        file_i           = 1;
        last_file_i      = 0;
        uniq_file_n      = 0;
        max_cdline_cnt_l = 1;
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
END {
        # scrap last file's result
        if ( last_file_i >= 1 ) {
                line_map[ARGV[last_file_i]] = cdline_cnt;
        }

        (uniq_file_n >= 2) ? sep = ":\t" : sep = ": ";

        magenta_f = "\033[35m"
        cyan_f    = "\033[36m"
        reset_f   = "\033[m"

        for (file in line_map) {
                printf("%s%s%s%s%s%"max_cdline_cnt_l"s\n",
                       magenta_f, file           ,
                       cyan_f   , sep            ,
                       reset_f  , line_map[file]);
        }

        file_number++
}