std::vector<char> value = libcomp::Convert::ToEncoding(
    @ENCODING@, @VAR_NAME@);

if(@FIXED_LENGTH@ > 0 && value.size() >= @FIXED_LENGTH@)
{
    value.resize(@FIXED_LENGTH@ - 1);
}

if(!value.empty())
{
    @ENCODESTREAM@.write(&value[0], static_cast<std::streamsize>(
        value.size()));
}